#include "Country.h"
#include "../../CK2World/Characters/Character.h"
#include "../../CK2World/Dynasties/Dynasty.h"
#include "../../CK2World/Provinces/Province.h"
#include "../../CK2World/Titles/Title.h"
#include "../../Mappers/ColorScraper/ColorScraper.h"
#include "../../Mappers/CultureMapper/CultureMapper.h"
#include "../../Mappers/GovernmentsMapper/GovernmentsMapper.h"
#include "../../Mappers/ProvinceMapper/ProvinceMapper.h"
#include "../../Mappers/ReligionMapper/ReligionMapper.h"
#include "../Province/EU4Province.h"
#include "Log.h"

EU4::Country::Country(std::string theTag, const std::string& filePath): tag(std::move(theTag))
{
	// Load from a country file, if one exists. Otherwise rely on defaults.
	const auto startPos = filePath.find("/countries");
	commonCountryFile = filePath.substr(startPos + 1, filePath.length() - startPos);
	details = CountryDetails(filePath);
}

void EU4::Country::loadHistory(const std::string& filePath)
{
	const auto startPos = filePath.find("/history");
	historyCountryFile = filePath.substr(startPos + 1, filePath.length() - startPos);
	details.parseHistory(filePath);
}

void EU4::Country::initializeFromTitle(std::string theTag,
	 std::shared_ptr<CK2::Title> theTitle,
	 const mappers::GovernmentsMapper& governmentsMapper,
	 const mappers::ReligionMapper& religionMapper,
	 const mappers::CultureMapper& cultureMapper,
	 const mappers::ProvinceMapper& provinceMapper,
	 const mappers::ColorScraper& colorScraper,
	 const mappers::LocalizationMapper& localizationMapper,
	 date theConversionDate)
{
	tag = std::move(theTag);
	conversionDate = theConversionDate;
	title.first = theTitle->getName();
	title.second = std::move(theTitle);
	if (commonCountryFile.empty()) commonCountryFile = "countries/" + title.first + ".txt";
	if (historyCountryFile.empty()) historyCountryFile = "history/countries/" + tag + " - " + title.first + ".txt";

	const auto& actualHolder = title.second->getHolder().second;

	// --------------- History section
	details.government.clear();
	details.reforms.clear();
	const auto& newGovernment = governmentsMapper.matchGovernment(actualHolder->getGovernment(), title.first);
	if (newGovernment) {
		details.government = newGovernment->first;
		if (!newGovernment->second.empty()) details.reforms.insert(newGovernment->second);
	} else {
		Log(LogLevel::Warning) << "No government match for " << actualHolder->getGovernment() << " for title: " << title.first << ", defaulting to monarchy.";
		details.government = "monarchy";
	}
	if (title.first.find("e_") == 0)
		details.governmentRank = 3;
	else if (title.first.find("k_") == 0)
		details.governmentRank = 2;
	else
		details.governmentRank = 1;
	// do we have a religion?
	std::string baseReligion;
	if (!actualHolder->getReligion().empty())
		baseReligion = actualHolder->getReligion();
	else if (!actualHolder->getDynasty().second->getReligion().empty())
		baseReligion = actualHolder->getDynasty().second->getReligion();
	else
		baseReligion = actualHolder->getCapitalProvince().second->getReligion();
	const auto& religionMatch = religionMapper.getEu4ReligionForCk2Religion(baseReligion);
	if (religionMatch)
		details.religion = *religionMatch;
	else {
		// We failed to get a religion. This is not an issue. We'll set it later from the majority of owned provinces.
		details.religion.clear();
	}
	const auto& capitalMatch = provinceMapper.getEU4ProvinceNumbers(actualHolder->getCapitalProvince().first);
	if (!capitalMatch.empty())
		details.capital = *capitalMatch.begin();
	else
		details.capital = 0; // We will see warning about this earlier, no need for more spam.
	// do we have a culture?
	std::string baseCulture;
	if (!actualHolder->getCulture().empty())
		baseCulture = actualHolder->getCulture();
	else if (!actualHolder->getDynasty().second->getCulture().empty())
		baseCulture = actualHolder->getDynasty().second->getCulture();
	else
		baseCulture = actualHolder->getCapitalProvince().second->getCulture();
	const auto& cultureMatch = cultureMapper.cultureMatch(baseCulture, details.religion, details.capital, tag);
	if (cultureMatch)
		details.primaryCulture = *cultureMatch;
	else {
		// We failed to get a primaryCulture. This is not an issue. We'll set it later from the majority of owned provinces.
		details.primaryCulture.clear();
	}
	if (!details.primaryCulture.empty()) {
		const auto& techMatch = cultureMapper.getTechGroup(details.primaryCulture);
		if (techMatch) details.technologyGroup = *techMatch;
	} // We will set it later if primaryCulture is unavailable at this stage.
	if (title.first.find("c_") == 0)
		details.fixedCapital = true;
	else
		details.fixedCapital = false;
	if (details.reforms.count("merchants_reform"))
		details.mercantilism = 25; // simulates merchant power of merchant republics.
	else
		details.mercantilism = 0;
	// Unit type should automatically match tech group. If not we'll add logic for it here.
	details.unitType.clear();
	// religious_school can be picked by player at leisure.
	details.religiousSchool.clear();
	// ditto for cults
	details.cults.clear();
	// We fill accepted cultures later, manually, once we can do a provincial census
	details.acceptedCultures.clear();
	if (details.reforms.count("monastic_order_reform"))
		details.armyProfessionalism = 0.05;
	else
		details.armyProfessionalism = 0;
	// We don't do historical rivalries or friendships at this stage.
	details.historicalRivals.clear();
	details.historicalFriends.clear();
	// We don't do national focus at this stage.
	details.nationalFocus.clear();
	// Piety is related to religious_school, which we don't set.
	details.piety = 0;
	// HRE Electorate is set later, once we can do a province/dev check.
	details.elector = false;
	if (title.second->isHREEmperor()) details.holyRomanEmperor = true;
	if (title.second->isInHRE()) details.inHRE = true;
	// ditto for secondary_religion and harmonized religions.
	details.secondaryReligion.clear();
	details.harmonizedReligions.clear();
	details.historicalScore = 0; // Not sure about this.

	// --------------  Common section
	if (!details.primaryCulture.empty()) {
		const auto& gfxmatch = cultureMapper.getGFX(details.primaryCulture);
		if (gfxmatch)
			details.graphicalCulture = *gfxmatch;
		else {
			Log(LogLevel::Warning) << tag << ": No gfx match for: " << details.primaryCulture << "! Substituting westerngfx!";
			details.graphicalCulture = "westerngfx";
		}
	} // We will set it later if primaryCulture is unavailable at this stage.
	if (!details.color) {
		if (title.second->getColor())
			details.color = title.second->getColor();
		else {
			const auto& colorMatch = colorScraper.getColorForTitle(title.first);
			if (colorMatch)
				details.color = *colorMatch;
			else
				Log(LogLevel::Warning) << tag << ": No color defined for title " << title.first << "!";
		}
	}
	// If we imported some revolutionary colors we'll keep them, otherwise, let's generate some.
	if (!details.revolutionaryColor) {
		details.revolutionaryColor = details.color;
		details.revolutionaryColor.RandomlyFlunctuate(20);
	}
	// If we imported historical idea groups, keeping them, otherwise blank.
	details.randomChance = false; // random chance related to RNW, wo it has no effect here.

	// If we imported historical units, keeping them, otherwise blank.

	details.monarchNames.clear();
	if (!title.second->getPreviousHolders().empty()) {
		for (const auto& previousHolder: title.second->getPreviousHolders()) {
			const auto& blockItr = details.monarchNames.find(previousHolder.second->getName());
			if (blockItr != details.monarchNames.end())
				blockItr->second.first++;
			else {
				auto female = previousHolder.second->isFemale();
				auto chance = 10;
				if (female) chance = -1;
				std::pair<int, int> newBlock = std::pair(1, chance);
				details.monarchNames.insert(std::pair(previousHolder.second->getName(), newBlock));
			}
		}
	}
	if (!title.second->getHolder().second->getCourtierNames().empty()) {
		for (const auto& courtier: title.second->getHolder().second->getCourtierNames()) {
			const auto& blockItr = details.monarchNames.find(courtier.first);
			if (blockItr == details.monarchNames.end()) {
				auto female = !courtier.second;
				auto chance = 0;
				if (female) chance = -1;
				std::pair<int, int> newBlock = std::pair(0, chance);
				details.monarchNames.insert(std::pair(courtier.first, newBlock));
			}
		}
	}
	 
	// If we imported leader_names, keeping them, otherwise blank.
	// If we imported ship_names, keeping them, otherwise blank.
	// If we imported army_names, keeping them, otherwise blank.
	// If we imported fleet_names, keeping them, otherwise blank.
	// If we imported preferred_religion, keeping it, otherwise blank.
	// If we imported colonial_parent, keeping it, otherwise blank.
	// If we imported special_unit_culture, keeping it, otherwise blank.
	// If we imported all_your_core_are_belong_to_us, keeping it, otherwise blank.
	// If we imported right_to_bear_arms, keeping it, otherwise blank.

	// --------------  Misc
	auto nameSet = false;
	auto nameLocalizationMatch = localizationMapper.getLocBlockForKey(title.first);
	if (nameLocalizationMatch) {
		localizations.insert(std::pair(tag, *nameLocalizationMatch));
		nameSet = true;
	}
	if (!nameSet && !title.second->getBaseTitle().first.empty()) { // see if we can match vs base title.
		auto baseTitleName = title.second->getBaseTitle().first;
		nameLocalizationMatch = localizationMapper.getLocBlockForKey(baseTitleName);
		if (nameLocalizationMatch) {
			localizations.insert(std::pair(tag, *nameLocalizationMatch));
			nameSet = true;
		}
	}
	if (!nameSet) {
		// Now get creative. This happens for c_titles that have localizations as b_title
		auto alternateName = title.second->getName();
		alternateName = "b_" + alternateName.substr(2, alternateName.length());
		nameLocalizationMatch = localizationMapper.getLocBlockForKey(alternateName);
		if (nameLocalizationMatch) {
			localizations.insert(std::pair(tag, *nameLocalizationMatch));
			nameSet = true;
		}
	}
	if (!nameSet) {
		// using capital province name?
		auto capitalName = actualHolder->getCapitalProvince().second->getName();
		if (!capitalName.empty()) {
			mappers::LocBlock newblock;
			newblock.english = capitalName;
			newblock.spanish = capitalName;
			newblock.french = capitalName;
			newblock.german = capitalName;
			localizations.insert(std::pair(tag, newblock));
			nameSet = true;
		}
	}
	// giving up.
	if (!nameSet) Log(LogLevel::Warning) << tag << " help with localization! " << title.first;

	auto adjSet = false;
	auto adjLocalizationMatch = localizationMapper.getLocBlockForKey(title.first + "_adj");
	if (adjLocalizationMatch) {
		localizations.insert(std::pair(tag + "_ADJ", *adjLocalizationMatch));
		adjSet = true;
	}
	if (!adjSet && !title.second->getBaseTitle().first.empty()) {
		// see if we can match vs base title.
		auto baseTitleAdj = title.second->getBaseTitle().first + "_adj";
		adjLocalizationMatch = localizationMapper.getLocBlockForKey(baseTitleAdj);
		if (adjLocalizationMatch) {
			localizations.insert(std::pair(tag + "_ADJ", *adjLocalizationMatch));
			adjSet = true;
		}
		if (!adjSet && !title.second->getBaseTitle().second->getBaseTitle().first.empty()) {
			// maybe basetitlebasetitle?
			baseTitleAdj = title.second->getBaseTitle().second->getBaseTitle().first + "_adj";
			adjLocalizationMatch = localizationMapper.getLocBlockForKey(baseTitleAdj);
			if (adjLocalizationMatch) {
				localizations.insert(std::pair(tag + "_ADJ", *adjLocalizationMatch));
				adjSet = true;
			}
		}
	}
	if (!adjSet) {
		// Maybe c_something?
		auto alternateAdj = title.second->getName() + "_adj";
		alternateAdj = "c_" + alternateAdj.substr(2, alternateAdj.length());
		adjLocalizationMatch = localizationMapper.getLocBlockForKey(alternateAdj);
		if (adjLocalizationMatch) {
			localizations.insert(std::pair(tag, *adjLocalizationMatch));
			adjSet = true;
		}
	}
	if (!adjSet) {
		// Or d_something?
		auto alternateAdj = title.second->getName() + "_adj";
		alternateAdj = "d_" + alternateAdj.substr(2, alternateAdj.length());
		adjLocalizationMatch = localizationMapper.getLocBlockForKey(alternateAdj);
		if (adjLocalizationMatch) {
			localizations.insert(std::pair(tag, *adjLocalizationMatch));
			adjSet = true;
		}
	}
	if (!adjSet) Log(LogLevel::Warning) << tag << " help with localization for adjective! " << title.first << "_adj?";

	// Rulers
	initializeRulers(religionMapper, cultureMapper);
}

void EU4::Country::initializeRulers(const mappers::ReligionMapper& religionMapper, const mappers::CultureMapper& cultureMapper)
{
	const auto& holder = title.second->getHolder().second;
	// Are we the ruler's primary title? (if he has any)
	if (!holder->getPrimaryTitle().first.empty() && title.first != holder->getPrimaryTitle().first) return; // PU's don't get monarchs.

	details.monarch.name = holder->getName();
	if (holder->getDynasty().first) details.monarch.dynasty = holder->getDynasty().second->getName();
	details.monarch.adm = std::min((holder->getSkills().stewardship + holder->getSkills().learning) / 4, 6);
	details.monarch.dip = std::min((holder->getSkills().diplomacy + holder->getSkills().intrigue) / 4, 6);
	details.monarch.mil = std::min((holder->getSkills().martial + holder->getSkills().learning) / 4, 6);
	details.monarch.birthDate = holder->getBirthDate();
	details.monarch.female = holder->isFemale();
	// religion and culture were already determining our country's primary culture and religion. If we set there, we'll copy here.
	if (!details.primaryCulture.empty()) details.monarch.culture = details.primaryCulture;
	if (!details.religion.empty()) details.monarch.religion = details.religion;
	details.monarch.isSet = true;

	if (!holder->getSpouses().empty()) {
		// What's the first spouse that's still alive?
		for (const auto& spouse: holder->getSpouses()) {
			if (spouse.second->getDeathDate() != date("1.1.1")) continue; // She's dead.
			details.queen.name = spouse.second->getName();
			if (spouse.second->getDynasty().first) details.queen.dynasty = spouse.second->getDynasty().second->getName();
			details.queen.adm = std::min((spouse.second->getSkills().stewardship + spouse.second->getSkills().learning) / 4, 6);
			details.queen.dip = std::min((spouse.second->getSkills().diplomacy + spouse.second->getSkills().intrigue) / 4, 6);
			details.queen.mil = std::min((spouse.second->getSkills().martial + spouse.second->getSkills().learning) / 4, 6);
			details.queen.birthDate = spouse.second->getBirthDate();
			details.queen.female = spouse.second->isFemale();
			if (spouse.second->getReligion().empty())
				details.queen.religion = details.monarch.religion; // taking a shortcut.
			else {
				const auto& religionMatch = religionMapper.getEu4ReligionForCk2Religion(spouse.second->getReligion());
				if (religionMatch) details.queen.religion = *religionMatch;
			}
			if (spouse.second->getCulture().empty())
				details.queen.culture = details.monarch.culture; // taking a shortcut.
			else {
				const auto& cultureMatch = cultureMapper.cultureMatch(spouse.second->getCulture(), details.queen.religion, 0, tag);
				if (cultureMatch) details.queen.culture = *cultureMatch;
			}
			details.queen.originCountry = tag;
			details.queen.deathDate = details.queen.birthDate;
			details.queen.deathDate.subtractYears(-60);
			details.queen.isSet = true;
		}
	}
}

void EU4::Country::setPrimaryCulture(const std::string& culture)
{
	details.primaryCulture = culture;
	if (details.monarch.isSet && details.monarch.culture.empty()) details.monarch.culture = culture;
	if (details.queen.isSet && details.queen.culture.empty()) details.queen.culture = culture;
}
void EU4::Country::setReligion(const std::string& religion)
{
	details.religion = religion;
	if (details.monarch.isSet && details.monarch.religion.empty()) details.monarch.religion = religion;
	if (details.queen.isSet && details.queen.religion.empty()) details.queen.religion = religion;
}

int EU4::Country::getDevelopment() const
{
	auto dev = 0;
	for (const auto& province: provinces) dev += province.second->getDev();
	return dev;
}

void EU4::Country::annexCountry(const std::pair<std::string, std::shared_ptr<Country>>& theCountry)
{
	// Provinces
	const auto& targetProvinces = theCountry.second->getProvinces();
	for (const auto& province: targetProvinces) {
		province.second->addCore(tag); // Adding, not replacing.
		province.second->setOwner(tag);
		province.second->setController(tag);
		provinces.insert(province);
	}
	theCountry.second->clearProvinces();

	// Vassals
	const auto& targetVassals = theCountry.second->getTitle().second->getGeneratedVassals();
	for (const auto& vassal: targetVassals) {
		vassal.second->registerGeneratedLiege(title);
		title.second->registerGeneratedVassal(vassal);
	}
	theCountry.second->getTitle().second->clearGeneratedVassals();

	// Bricking the title -> eu4tag is not necessary and not desirable. As soon as the country has 0 provinces, it's effectively dead.
}
