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
	 const mappers::LocalizationMapper& localizationMapper)
{
	tag = std::move(theTag);
	title = std::move(theTitle);
	if (commonCountryFile.empty()) commonCountryFile = "countries/" + title->getName() + ".txt";
	if (historyCountryFile.empty()) historyCountryFile = "history/countries/" + tag + " - " + title->getName() + ".txt";

	const auto& actualHolder = title->getHolder().second;

	// --------------- History section
	details.government.clear();
	details.reforms.clear();
	const auto& newGovernment = governmentsMapper.matchGovernment(actualHolder->getGovernment(), title->getName());
	if (newGovernment) {
		details.government = newGovernment->first;
		if (!newGovernment->second.empty()) details.reforms.insert(newGovernment->second);
	} else {
		Log(LogLevel::Warning) << "No government match for " << actualHolder->getGovernment() << " for title: " << title->getName() << ", defaulting to monarchy.";
		details.government = "monarchy";
	}
	if (title->getName().find("e_") == 0)
		details.governmentRank = 3;
	else if (title->getName().find("k_") == 0)
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
		Log(LogLevel::Warning) << tag << ": No religion match for: " << baseReligion << " holder: " << actualHolder->getID() << "! Substituting noreligion!";
		details.religion = "noreligion";
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
		Log(LogLevel::Warning) << tag << ": No culture match for: " << baseCulture << " holder: " << actualHolder->getID() << "! Substituting noculture!";
		details.primaryCulture = "noculture";
	}
	const auto& techMatch = cultureMapper.getTechGroup(details.primaryCulture);
	if (techMatch)
		details.technologyGroup = *techMatch;
	else {
		// disabled during testing
		// Log(LogLevel::Warning) << tag << ": No tech group match for: " << details.primaryCulture << "! Substituting western!";
		// details.technologyGroup = "western";
		details.technologyGroup = "high_american";
	}
	if (title->getName().find("c_") == 0)
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
	// ditto for secondary_religion and harmonized religions.
	details.secondaryReligion.clear();
	details.harmonizedReligions.clear();

	// --------------  Common section
	const auto& gfxmatch = cultureMapper.getGFX(details.primaryCulture);
	if (gfxmatch)
		details.graphicalCulture = *gfxmatch;
	else {
		Log(LogLevel::Warning) << tag << ": No gfx match for: " << details.primaryCulture << "! Substituting westerngfx!";
		details.graphicalCulture = "westerngfx";
	}
	if (title->getColor())
		details.color = title->getColor();
	else {
		const auto& colorMatch = colorScraper.getColorForTitle(title->getName());
		if (colorMatch)
			details.color = *colorMatch;
		else
			Log(LogLevel::Warning) << tag << ": No color defined for title " << title->getName() << "!";
	}
	// If we imported some revolutionary colors we'll keep them, otherwise, let's generate some.
	if (!details.revolutionaryColor) {
		details.revolutionaryColor = details.color;
		details.revolutionaryColor.RandomlyFlunctuate(20);
	}
	// If we imported historical idea groups, keeping them, otherwise blank.
	details.randomChance = false; // random chance related to RNW, wo it has no effect here.

	// If we imported historical units, keeping them, otherwise blank.
	// monarch_names will need some doing
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
	auto nameLocalizationMatch = localizationMapper.getLocBlockForKey(title->getName());
	if (nameLocalizationMatch) {
		localizations.insert(std::pair(tag, *nameLocalizationMatch));
		nameSet = true;
	}
	if (!nameSet && !title->getBaseTitle().first.empty()) { // see if we can match vs base title.
		auto baseTitleName = title->getBaseTitle().first;
		nameLocalizationMatch = localizationMapper.getLocBlockForKey(baseTitleName);
		if (nameLocalizationMatch) {
			localizations.insert(std::pair(tag, *nameLocalizationMatch));
			nameSet = true;
		}
	}
	if (!nameSet) {
		// Now get creative. This happens for c_titles that have localizations as b_title
		auto alternateName = title->getName();
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
	if (!nameSet) Log(LogLevel::Warning) << tag << " help with localization! " << title->getName();

	auto adjSet = false;
	auto adjLocalizationMatch = localizationMapper.getLocBlockForKey(title->getName() + "_adj");
	if (adjLocalizationMatch) {
		localizations.insert(std::pair(tag + "_ADJ", *adjLocalizationMatch));
		adjSet = true;
	}
	if (!adjSet && !title->getBaseTitle().first.empty()) {
		// see if we can match vs base title.
		auto baseTitleAdj = title->getBaseTitle().first + "_adj";
		adjLocalizationMatch = localizationMapper.getLocBlockForKey(baseTitleAdj);
		if (adjLocalizationMatch) {
			localizations.insert(std::pair(tag + "_ADJ", *adjLocalizationMatch));
			adjSet = true;
		}
	}
	if (!adjSet) Log(LogLevel::Warning) << tag << " help with localization for adjective! " << title->getName() << "_adj?";
}
