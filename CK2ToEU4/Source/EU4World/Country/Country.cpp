#include "Country.h"
#include "../../CK2World/Characters/Character.h"
#include "../../CK2World/Dynasties/Dynasty.h"
#include "../../CK2World/Provinces/Province.h"
#include "../../CK2World/Titles/Title.h"
#include "../../Common/CommonFunctions.h"
#include "../../Mappers/ColorScraper/ColorScraper.h"
#include "../../Mappers/CultureMapper/CultureMapper.h"
#include "../../Mappers/GovernmentsMapper/GovernmentsMapper.h"
#include "../../Mappers/ProvinceMapper/ProvinceMapper.h"
#include "../../Mappers/ReligionMapper/ReligionMapper.h"
#include "../../Mappers/RulerPersonalitiesMapper/RulerPersonalitiesMapper.h"
#include "../Province/EU4Province.h"
#include "Log.h"
#include <cmath>

EU4::Country::Country(std::string theTag, const std::string& filePath): tag(std::move(theTag))
{
	// Load from a country file, if one exists. Otherwise rely on defaults.
	const auto startPos = filePath.find("/countries");
	commonCountryFile = filePath.substr(startPos + 1, filePath.length() - startPos);
	details = CountryDetails(filePath);

	// We also must set a dummy history filepath for those countries that don't actually have a history file.
	const auto lastslash = filePath.find_last_of('/');
	const auto rawname = filePath.substr(lastslash + 1, filePath.length());

	historyCountryFile = "history/countries/" + tag + " - " + rawname;
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
	 const mappers::RulerPersonalitiesMapper& rulerPersonalitiesMapper,
	 date theConversionDate)
{
	tag = std::move(theTag);
	conversionDate = theConversionDate;
	title.first = theTitle->getName();
	title.second = std::move(theTitle);
	if (commonCountryFile.empty())
		commonCountryFile = "countries/" + title.first + ".txt";
	if (historyCountryFile.empty())
		historyCountryFile = "history/countries/" + tag + " - " + title.first + ".txt";

	const auto& actualHolder = title.second->getHolder().second;
	if (actualHolder->getDynasty().first)
		details.dynastyID = actualHolder->getDynasty().first;

	// --------------- History section
	details.government.clear();
	details.reforms.clear();
	const auto& newGovernment = governmentsMapper.matchGovernment(actualHolder->getGovernment(), title.first);
	if (newGovernment)
	{
		details.government = newGovernment->first;
		if (!newGovernment->second.empty())
			details.reforms.insert(newGovernment->second);
	}
	else
	{
		Log(LogLevel::Warning) << "No government match for " << actualHolder->getGovernment() << " for title: " << title.first << ", defaulting to monarchy.";
		details.government = "monarchy";
	}
	if (title.second->getSuccessionLaw() == "feudal_elective" && tag != "ROM" && tag != "HRE" && tag != "BYZ")
	{
		details.reforms = {"elective_monarchy"};
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
	else
		baseReligion = actualHolder->getCapitalProvince().second->getReligion();
	const auto& religionMatch = religionMapper.getEu4ReligionForCk2Religion(baseReligion);
	if (religionMatch)
		details.religion = *religionMatch;
	else
	{
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
	else
		baseCulture = actualHolder->getCapitalProvince().second->getCulture();
	const auto& cultureMatch = cultureMapper.cultureMatch(baseCulture, details.religion, details.capital, tag);
	if (cultureMatch)
		details.primaryCulture = *cultureMatch;
	else
	{
		// We failed to get a primaryCulture. This is not an issue. We'll set it later from the majority of owned provinces.
		details.primaryCulture.clear();
	}
	if (!details.primaryCulture.empty())
	{
		const auto& techMatch = cultureMapper.getTechGroup(details.primaryCulture);
		if (techMatch)
			details.technologyGroup = *techMatch;
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
	if (title.second->isHREEmperor())
		details.holyRomanEmperor = true;
	if (title.second->isInHRE())
		details.inHRE = true;
	// ditto for secondary_religion and harmonized religions.
	details.secondaryReligion.clear();
	details.harmonizedReligions.clear();
	details.historicalScore = 0; // Not sure about this.

	// --------------  Common section
	if (!details.primaryCulture.empty())
	{
		const auto& gfxmatch = cultureMapper.getGFX(details.primaryCulture);
		if (gfxmatch)
			details.graphicalCulture = *gfxmatch;
		else
		{
			Log(LogLevel::Warning) << tag << ": No gfx match for: " << details.primaryCulture << "! Substituting westerngfx!";
			details.graphicalCulture = "westerngfx";
		}
	} // We will set it later if primaryCulture is unavailable at this stage.
	if (!details.color)
	{
		if (title.second->getColor())
			details.color = title.second->getColor();
		else
		{
			const auto& colorMatch = colorScraper.getColorForTitle(title.first);
			if (colorMatch)
				details.color = *colorMatch;
			else
				Log(LogLevel::Warning) << tag << ": No color defined for title " << title.first << "!";
		}
	}
	// If we imported some revolutionary colors we'll keep them, otherwise we're ignoring this.
	// If we imported historical idea groups, keeping them, otherwise blank.
	details.randomChance = false; // random chance related to RNW, wo it has no effect here.

	// If we imported historical units, keeping them, otherwise blank.

	details.monarchNames.clear();
	if (!title.second->getPreviousHolders().empty())
	{
		for (const auto& previousHolder: title.second->getPreviousHolders())
		{
			const auto& blockItr = details.monarchNames.find(previousHolder.second->getName());
			if (blockItr != details.monarchNames.end())
				blockItr->second.first++;
			else
			{
				auto female = previousHolder.second->isFemale();
				auto chance = 10;
				if (female)
					chance = -1;
				std::pair<int, int> newBlock = std::pair(1, chance);
				details.monarchNames.insert(std::pair(previousHolder.second->getName(), newBlock));
			}
		}
	}
	if (!title.second->getHolder().second->getCourtierNames().empty())
	{
		for (const auto& courtier: title.second->getHolder().second->getCourtierNames())
		{
			const auto& blockItr = details.monarchNames.find(courtier.first);
			if (blockItr == details.monarchNames.end())
			{
				auto female = !courtier.second;
				auto chance = 0;
				if (female)
					chance = -1;
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

	if (actualHolder->getWealth())
		details.addTreasury = lround(7 * log2(actualHolder->getWealth()));
	if (actualHolder->getPrestige())
		details.addPrestige = -50 + std::max(-50, static_cast<int>(lround(15 * log2(actualHolder->getPrestige() - 100) - 50)));
	if (actualHolder->hasLoan())
		details.loan = true;
	if (actualHolder->isExcommunicated())
		details.excommunicated = true;

	auto nameSet = false;
	// Override for muslims kingdoms/empires.
	std::set<std::string> muslimReligions = {"sunni", "zikri", "yazidi", "ibadi", "kharijite", "shiite", "druze", "hurufi", "qarmatian"};
	if (muslimReligions.count(details.religion) && actualHolder->getDynasty().first && !actualHolder->getDynasty().second->getName().empty() &&
		 title.first != "k_rum" && title.first != "k_israel" && title.first != "e_india" && (title.first.find("e_") == 0 || title.first.find("k_") == 0))
	{
		const auto& dynastyName = actualHolder->getDynasty().second->getName();
		mappers::LocBlock newblock;
		newblock.english = dynastyName;
		newblock.spanish = dynastyName;
		newblock.french = dynastyName;
		newblock.german = dynastyName;
		localizations.insert(std::pair(tag, newblock));
		nameSet = true;
	}
	if (!nameSet && !title.second->getDisplayName().empty())
	{
		mappers::LocBlock newblock;
		newblock.english = title.second->getDisplayName();
		newblock.spanish = title.second->getDisplayName();
		newblock.french = title.second->getDisplayName();
		newblock.german = title.second->getDisplayName();
		localizations.insert(std::pair(tag, newblock));
		nameSet = true;
	}
	if (!nameSet)
	{
		auto nameLocalizationMatch = localizationMapper.getLocBlockForKey(title.first);
		if (nameLocalizationMatch)
		{
			localizations.insert(std::pair(tag, *nameLocalizationMatch));
			nameSet = true;
		}
	}
	if (!nameSet && !title.second->getBaseTitle().first.empty())
	{ // see if we can match vs base title.
		auto baseTitleName = title.second->getBaseTitle().first;
		auto nameLocalizationMatch = localizationMapper.getLocBlockForKey(baseTitleName);
		if (nameLocalizationMatch)
		{
			localizations.insert(std::pair(tag, *nameLocalizationMatch));
			nameSet = true;
		}
	}
	if (!nameSet)
	{
		// Now get creative. This happens for c_titles that have localizations as b_title
		auto alternateName = title.second->getName();
		alternateName = "b_" + alternateName.substr(2, alternateName.length());
		auto nameLocalizationMatch = localizationMapper.getLocBlockForKey(alternateName);
		if (nameLocalizationMatch)
		{
			localizations.insert(std::pair(tag, *nameLocalizationMatch));
			nameSet = true;
		}
	}
	if (!nameSet)
	{
		// using capital province name?
		auto capitalName = actualHolder->getCapitalProvince().second->getName();
		if (!capitalName.empty())
		{
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
	if (!nameSet)
		Log(LogLevel::Warning) << tag << " help with localization! " << title.first;

	auto adjSet = false;
	if (muslimReligions.count(details.religion) && actualHolder->getDynasty().first && !actualHolder->getDynasty().second->getName().empty() &&
		 title.first != "k_rum" && title.first != "k_israel" && title.first != "e_india" && (title.first.find("e_") == 0 || title.first.find("k_") == 0))
	{
		const auto& dynastyName = actualHolder->getDynasty().second->getName();
		mappers::LocBlock newblock;
		newblock.english = dynastyName + "s'"; // plural so Ottomans' Africa
		newblock.spanish = "de los " + dynastyName;
		newblock.french = "des " + dynastyName;
		newblock.german = dynastyName + "-";
		localizations.insert(std::pair(tag + "_ADJ", newblock));
		adjSet = true;
	}
	if (!adjSet && !title.second->getDisplayName().empty())
	{
		mappers::LocBlock newblock;
		newblock.english = title.second->getDisplayName() + "'s"; // singular Nordarike's Africa
		newblock.spanish = "de " + title.second->getDisplayName();
		newblock.french = "de " + title.second->getDisplayName();
		newblock.german = title.second->getDisplayName() + "s";
		localizations.insert(std::pair(tag + "_ADJ", newblock));
		adjSet = true;
	}
	if (!adjSet)
	{
		auto adjLocalizationMatch = localizationMapper.getLocBlockForKey(title.first + "_adj");
		if (adjLocalizationMatch)
		{
			localizations.insert(std::pair(tag + "_ADJ", *adjLocalizationMatch));
			adjSet = true;
		}
	}
	if (!adjSet && !title.second->getBaseTitle().first.empty())
	{
		// see if we can match vs base title.
		auto baseTitleAdj = title.second->getBaseTitle().first + "_adj";
		auto adjLocalizationMatch = localizationMapper.getLocBlockForKey(baseTitleAdj);
		if (adjLocalizationMatch)
		{
			localizations.insert(std::pair(tag + "_ADJ", *adjLocalizationMatch));
			adjSet = true;
		}
		if (!adjSet && !title.second->getBaseTitle().second->getBaseTitle().first.empty())
		{
			// maybe basetitlebasetitle?
			baseTitleAdj = title.second->getBaseTitle().second->getBaseTitle().first + "_adj";
			adjLocalizationMatch = localizationMapper.getLocBlockForKey(baseTitleAdj);
			if (adjLocalizationMatch)
			{
				localizations.insert(std::pair(tag + "_ADJ", *adjLocalizationMatch));
				adjSet = true;
			}
		}
	}
	if (!adjSet)
	{
		// Maybe c_something?
		auto alternateAdj = title.second->getName() + "_adj";
		alternateAdj = "c_" + alternateAdj.substr(2, alternateAdj.length());
		auto adjLocalizationMatch = localizationMapper.getLocBlockForKey(alternateAdj);
		if (adjLocalizationMatch)
		{
			localizations.insert(std::pair(tag, *adjLocalizationMatch));
			adjSet = true;
		}
	}
	if (!adjSet)
	{
		// Or d_something?
		auto alternateAdj = title.second->getName() + "_adj";
		alternateAdj = "d_" + alternateAdj.substr(2, alternateAdj.length());
		auto adjLocalizationMatch = localizationMapper.getLocBlockForKey(alternateAdj);
		if (adjLocalizationMatch)
		{
			localizations.insert(std::pair(tag, *adjLocalizationMatch));
			adjSet = true;
		}
	}
	if (!adjSet)
		Log(LogLevel::Warning) << tag << " help with localization for adjective! " << title.first << "_adj?";

	// Rulers
	initializeRulers(religionMapper, cultureMapper, rulerPersonalitiesMapper);
}

bool EU4::Country::verifyCapital(const mappers::ProvinceMapper& provinceMapper)
{
	// We have set a provisionary capital earlier, but now we can check if it's valid.
	if (title.first.empty())
		return false;
	if (provinces.empty())
		return false;
	if (details.capital && provinces.count(details.capital))
		return false;

	const auto& actualHolder = title.second->getHolder().second;
	const auto& capitalMatch = provinceMapper.getEU4ProvinceNumbers(actualHolder->getCapitalProvince().first);
	if (!capitalMatch.empty())
	{
		for (const auto& provinceID: capitalMatch)
		{
			if (provinces.count(provinceID))
			{
				details.capital = provinceID;
				return true;
			}
		}
	}
	// Use any other province.
	details.capital = provinces.begin()->second->getProvinceID();
	return true;
}


void EU4::Country::initializeAdvisers(const mappers::ReligionMapper& religionMapper, const mappers::CultureMapper& cultureMapper)
{
	// We're doing this one separate from initial country generation so that country's primary culture and religion may have had time to get
	// initialized.
	if (title.first.empty() || !title.second->getHolder().first)
		return; // Vanilla and the dead do not get these.
	const auto& holder = title.second->getHolder().second;
	if (!holder->getPrimaryTitle().first.empty() && title.first != holder->getPrimaryTitle().first)
		return; // PU's don't get advisors on secondary countries.

	for (const auto& adviser: holder->getAdvisers())
	{
		if (adviser.second->isSpent())
			continue;
		Character newAdviser;
		newAdviser.name = adviser.second->getName();
		if (adviser.second->getDynasty().first)
			newAdviser.name += " " + adviser.second->getDynasty().second->getName();
		if (details.capital)
			newAdviser.location = details.capital;
		if (adviser.second->getJob() == "job_spiritual")
		{
			newAdviser.type = "theologian";
			newAdviser.skill = std::min(3, std::max(1, adviser.second->getSkills().learning / 4));
		}
		else if (adviser.second->getJob() == "job_marshal")
		{
			newAdviser.type = "army_organiser";
			newAdviser.skill = std::min(3, std::max(1, adviser.second->getSkills().martial / 4));
		}
		else if (adviser.second->getJob() == "job_spymaster")
		{
			newAdviser.type = "spymaster";
			newAdviser.skill = std::min(3, std::max(1, adviser.second->getSkills().intrigue / 4));
		}
		else if (adviser.second->getJob() == "job_treasurer")
		{
			newAdviser.type = "treasurer";
			newAdviser.skill = std::min(3, std::max(1, adviser.second->getSkills().intrigue / 4));
		}
		else if (adviser.second->getJob() == "job_chancellor")
		{
			newAdviser.type = "statesman";
			newAdviser.skill = std::min(3, std::max(1, adviser.second->getSkills().diplomacy / 4));
		}
		else
		{
			Log(LogLevel::Warning) << "Unrecognized job for " << adviser.first << ": " << adviser.second->getJob();
			continue;
		}
		newAdviser.id = adviser.first;
		newAdviser.appearDate = adviser.second->getBirthDate();
		newAdviser.appearDate.subtractYears(-16);
		newAdviser.deathDate = adviser.second->getBirthDate();
		newAdviser.deathDate.subtractYears(-65);
		newAdviser.female = adviser.second->isFemale();
		if (adviser.second->getReligion().empty())
			newAdviser.religion = details.monarch.religion; // taking a shortcut.
		else
		{
			const auto& religionMatch = religionMapper.getEu4ReligionForCk2Religion(adviser.second->getReligion());
			if (religionMatch)
				newAdviser.religion = *religionMatch;
		}
		if (newAdviser.religion.empty())
			continue;
		if (adviser.second->getCulture().empty())
			newAdviser.culture = details.monarch.culture; // taking a shortcut.
		else
		{
			const auto& cultureMatch = cultureMapper.cultureMatch(adviser.second->getCulture(), newAdviser.religion, 0, tag);
			if (cultureMatch)
				newAdviser.culture = *cultureMatch;
		}
		if (newAdviser.culture.empty())
			continue;
		if (newAdviser.religion == "jewish")
			newAdviser.discount = true; // Tradeoff for not being promotable.
		details.advisers.emplace_back(newAdviser);
		adviser.second->setSpent();
	}
}


void EU4::Country::initializeRulers(const mappers::ReligionMapper& religionMapper,
	 const mappers::CultureMapper& cultureMapper,
	 const mappers::RulerPersonalitiesMapper& rulerPersonalitiesMapper)
{
	const auto& holder = title.second->getHolder().second;
	// Are we the ruler's primary title? (if he has any)
	if (!holder->getPrimaryTitle().first.empty() && title.first != holder->getPrimaryTitle().first)
		return; // PU's don't get monarchs.

	// Determine regnalness.
	if (details.government != "republic" && !details.monarchNames.empty())
	{
		auto const& theName = holder->getName();
		std::string roman;
		const auto& nameItr = details.monarchNames.find(theName);
		if (nameItr != details.monarchNames.end())
		{
			const auto regnal = nameItr->second.first;
			if (regnal > 1)
			{
				roman = cardinalToRoman(regnal);
				roman = " " + roman;
			}
		}
		details.monarch.name = holder->getName() + roman;
	}
	else
	{
		details.monarch.name = holder->getName();
	}
	if (holder->getDynasty().first)
		details.monarch.dynasty = holder->getDynasty().second->getName();
	details.monarch.adm = std::min((holder->getSkills().stewardship + holder->getSkills().learning) / 4, 6);
	details.monarch.dip = std::min((holder->getSkills().diplomacy + holder->getSkills().intrigue) / 4, 6);
	details.monarch.mil = std::min((holder->getSkills().martial + holder->getSkills().learning) / 4, 6);
	details.monarch.birthDate = holder->getBirthDate();
	details.monarch.female = holder->isFemale();
	// religion and culture were already determining our country's primary culture and religion. If we set there, we'll copy here.
	if (!details.primaryCulture.empty())
		details.monarch.culture = details.primaryCulture;
	if (!details.religion.empty())
		details.monarch.religion = details.religion;
	details.monarch.personalities = rulerPersonalitiesMapper.evaluatePersonalities(title.second->getHolder());
	details.monarch.isSet = true;

	if (!holder->getSpouses().empty())
	{
		// What's the first spouse that's still alive?
		for (const auto& spouse: holder->getSpouses())
		{
			if (spouse.second->getDeathDate() != date("1.1.1"))
				continue; // She's dead.
			details.queen.name = spouse.second->getName();
			if (spouse.second->getDynasty().first)
				details.queen.dynasty = spouse.second->getDynasty().second->getName();
			details.queen.adm = std::min((spouse.second->getSkills().stewardship + spouse.second->getSkills().learning) / 4, 6);
			details.queen.dip = std::min((spouse.second->getSkills().diplomacy + spouse.second->getSkills().intrigue) / 4, 6);
			details.queen.mil = std::min((spouse.second->getSkills().martial + spouse.second->getSkills().learning) / 4, 6);
			details.queen.birthDate = spouse.second->getBirthDate();
			details.queen.female = spouse.second->isFemale();
			if (spouse.second->getReligion().empty())
				details.queen.religion = details.monarch.religion; // taking a shortcut.
			else
			{
				const auto& religionMatch = religionMapper.getEu4ReligionForCk2Religion(spouse.second->getReligion());
				if (religionMatch)
					details.queen.religion = *religionMatch;
			}
			if (spouse.second->getCulture().empty())
				details.queen.culture = details.monarch.culture; // taking a shortcut.
			else
			{
				const auto& cultureMatch = cultureMapper.cultureMatch(spouse.second->getCulture(), details.queen.religion, 0, tag);
				if (cultureMatch)
					details.queen.culture = *cultureMatch;
			}
			details.queen.originCountry = tag;
			details.queen.deathDate = details.queen.birthDate;
			details.queen.deathDate.subtractYears(-60);
			details.queen.personalities = rulerPersonalitiesMapper.evaluatePersonalities(spouse);
			details.queen.isSet = true;
			break;
		}
	}

	if (holder->getHeir().first)
	{
		const auto& heir = holder->getHeir();
		details.heir.name = heir.second->getName();
		// We're setting future regnalness
		if (details.government != "republic" && !details.monarchNames.empty())
		{
			auto const& theName = heir.second->getName();
			std::string roman;
			const auto& nameItr = details.monarchNames.find(theName);
			if (nameItr != details.monarchNames.end())
			{
				const auto regnal = nameItr->second.first;
				if (regnal >= 1)
				{
					roman = cardinalToRoman(regnal + 1);
					roman = " " + roman;
				}
			}
			details.heir.monarchName = heir.second->getName() + roman;
		}
		if (heir.second->getDynasty().first)
			details.heir.dynasty = heir.second->getDynasty().second->getName();
		details.heir.adm = std::min((heir.second->getSkills().stewardship + heir.second->getSkills().learning) / 3, 6);
		details.heir.dip = std::min((heir.second->getSkills().diplomacy + heir.second->getSkills().intrigue) / 3, 6);
		details.heir.mil = std::min((heir.second->getSkills().martial + heir.second->getSkills().learning) / 3, 6);
		details.heir.birthDate = heir.second->getBirthDate();
		details.heir.female = heir.second->isFemale();
		if (heir.second->getReligion().empty())
			details.heir.religion = details.monarch.religion; // taking a shortcut.
		else
		{
			const auto& religionMatch = religionMapper.getEu4ReligionForCk2Religion(heir.second->getReligion());
			if (religionMatch)
				details.heir.religion = *religionMatch;
		}
		if (heir.second->getCulture().empty())
			details.heir.culture = details.monarch.culture; // taking a shortcut.
		else
		{
			const auto& cultureMatch = cultureMapper.cultureMatch(heir.second->getCulture(), details.heir.religion, 0, tag);
			if (cultureMatch)
				details.heir.culture = *cultureMatch;
		}
		details.heir.deathDate = details.heir.birthDate;
		details.heir.deathDate.subtractYears(-65);
		details.heir.claim = 89; // good enough?
		details.heir.personalities = rulerPersonalitiesMapper.evaluatePersonalities(heir);
		details.heir.isSet = true;
	}

	if (conversionDate.diffInYears(details.monarch.birthDate) < 16)
	{
		details.heir = details.monarch;
		details.heir.monarchName.clear();
		details.heir.deathDate = details.heir.birthDate;
		details.heir.deathDate.subtractYears(-65);
		details.heir.claim = 89; // good enough?
		details.heir.adm = std::min(details.heir.adm + 2, 6);
		details.heir.mil = std::min(details.heir.mil + 2, 6);
		details.heir.dip = std::min(details.heir.dip + 2, 6);
		details.heir.personalities.clear();

		details.monarch.name = "(Regency Council)";
		details.monarch.regency = true;
		details.monarch.birthDate = date("1.1.1");
		details.monarch.female = false;
		details.monarch.dynasty.clear();
		details.monarch.personalities.clear();
	}
}

void EU4::Country::setPrimaryCulture(const std::string& culture)
{
	details.primaryCulture = culture;
	if (details.monarch.isSet && details.monarch.culture.empty())
		details.monarch.culture = culture;
	if (details.queen.isSet && details.queen.culture.empty())
		details.queen.culture = culture;
	if (details.heir.isSet && details.heir.culture.empty())
		details.heir.culture = culture;
}
void EU4::Country::setReligion(const std::string& religion)
{
	details.religion = religion;
	if (details.monarch.isSet && details.monarch.religion.empty())
		details.monarch.religion = religion;
	if (details.queen.isSet && details.queen.religion.empty())
		details.queen.religion = religion;
	if (details.heir.isSet && details.heir.religion.empty())
		details.heir.religion = religion;
}

int EU4::Country::getDevelopment() const
{
	auto dev = 0;
	for (const auto& province: provinces)
		dev += province.second->getDev();
	return dev;
}

void EU4::Country::annexCountry(const std::pair<std::string, std::shared_ptr<Country>>& theCountry)
{
	// Provinces
	const auto& targetProvinces = theCountry.second->getProvinces();
	for (const auto& province: targetProvinces)
	{
		province.second->addCore(tag); // Adding, not replacing.
		province.second->setOwner(tag);
		province.second->setController(tag);
		provinces.insert(province);
	}
	theCountry.second->clearProvinces();

	// Vassals
	const auto& targetVassals = theCountry.second->getTitle().second->getGeneratedVassals();
	for (const auto& vassal: targetVassals)
	{
		vassal.second->registerGeneratedLiege(title);
		title.second->registerGeneratedVassal(vassal);
	}
	theCountry.second->getTitle().second->clearGeneratedVassals();

	// Bricking the title -> eu4tag is not necessary and not desirable. As soon as the country has 0 provinces, it's effectively dead.
}
