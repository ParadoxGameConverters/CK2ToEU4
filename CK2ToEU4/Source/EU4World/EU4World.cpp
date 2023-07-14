#include "EU4World.h"
#include "../CK2World/Characters/Character.h"
#include "../CK2World/Dynasties/Dynasty.h"
#include "../CK2World/Offmaps/Offmap.h"
#include "../CK2World/Provinces/Barony.h"
#include "../CK2World/Titles/Title.h"
#include "../Configuration/Configuration.h"
#include "CommonFunctions.h"
#include "Log.h"
#include "OSCompatibilityLayer.h"
#include <cmath>
#include <filesystem>
#include <fstream>
namespace fs = std::filesystem;

EU4::World::World(const CK2::World& sourceWorld, const Configuration& theConfiguration, const commonItems::ConverterVersion& converterVersion)
{
	Log(LogLevel::Info) << "*** Hello EU4, let's get painting. ***";
	// Do we have an override mod?
	std::string overrideModPath;
	for (const auto& mod: sourceWorld.getMods())
		if (mod.name == "CleanSlate")
		{
			overrideModPath = "CleanSlate";
			cleanslate = true;
		}
		else if (mod.name == "Tianxia: Silk Road Expansion")
		{
			overrideModPath = "Tianxia";
			tianxia = true;
		}

	cultureMapper.initCultureMapper(overrideModPath);
	governmentsMapper.initGovernmentsMapper(overrideModPath);
	rulerPersonalitiesMapper.initRulerPersonalitiesMapper(overrideModPath);
	religionMapper.initReligionMapper(overrideModPath);
	titleTagMapper.initTitleTagMapper(overrideModPath);

	// Scraping localizations from CK2 so we may know proper names for our countries.
	localizationMapper.scrapeLocalizations(theConfiguration, sourceWorld.getMods());

	// Scrape Primary Tags for nationalities
	primaryTagMapper.loadPrimaryTags(theConfiguration);
	Log(LogLevel::Progress) << "50 %";

	// Ditto for colors - these only apply on non-eu4 countries.
	scrapeColors(theConfiguration, sourceWorld);
	Log(LogLevel::Progress) << "51 %";

	// This is our region mapper for eu4 regions, areas and superRegions. It's a pointer because we need
	// to embed it into every cultureMapper individual mapping. It works faster that way.
	regionMapper = std::make_shared<mappers::RegionMapper>();
	regionMapper->loadRegions(theConfiguration);
	Log(LogLevel::Progress) << "52 %";

	// And this is the cultureMapper. It's of vital importance.
	cultureMapper.loadRegionMapper(regionMapper);
	Log(LogLevel::Progress) << "53 %";

	// This is a valid province scraper. It looks at eu4 map data and notes which eu4 provinces are in fact valid.
	// ... It's not used at all.
	provinceMapper = std::make_unique<mappers::ProvinceMapper>(sourceWorld.getMods(), overrideModPath);
	provinceMapper->determineValidProvinces(theConfiguration);
	Log(LogLevel::Progress) << "54 %";

	// We start conversion by importing vanilla eu4 countries, history and common sections included.
	// We'll overwrite some of them with ck2 imports.
	importVanillaCountries(theConfiguration.getEU4Path(), sourceWorld.isInvasion());
	Log(LogLevel::Progress) << "55 %";

	// Which happens now. Translating incoming titles into EU4 tags, with new tags being added to our countries.
	importCK2Countries(theConfiguration.getStartDateOption(), sourceWorld);
	Log(LogLevel::Progress) << "56 %";

	// Now we can deal with provinces since we know to whom to assign them. We first import vanilla province data.
	// Some of it will be overwritten, but not all.
	importVanillaProvinces(theConfiguration.getEU4Path(), sourceWorld.isInvasion());
	Log(LogLevel::Progress) << "57 %";

	// We can link provinces to regionMapper's bindings, though this is not used at the moment.
	regionMapper->linkProvinces(provinces);
	Log(LogLevel::Progress) << "58 %";

	// Next we import ck2 provinces and translate them ontop a significant part of all imported provinces.
	importCK2Provinces(sourceWorld);
	Log(LogLevel::Progress) << "59 %";

	// With Ck2 provinces linked to those eu4 provinces they affect, we can adjust eu4 province dev values.
	if (theConfiguration.getDevelopment() == Configuration::DEVELOPMENT::IMPORT)
		alterProvinceDevelopment();
	Log(LogLevel::Progress) << "60 %";

	// We then link them to their respective countries. Those countries that end up with 0 provinces are defacto dead.
	linkProvincesToCountries();
	Log(LogLevel::Progress) << "61 %";

	// Country capitals are fuzzy, and need checking if we assigned them to some other country during mapping.
	verifyCapitals();
	Log(LogLevel::Progress) << "62 %";

	// This step is important. CK2 data is sketchy and not every character or province has culture/religion data.
	// For those, we look at vanilla provinces and override missing bits with vanilla setup. Yeah, a bit more sunni in
	// hordeland, but it's fine.
	verifyReligionsAndCultures();
	Log(LogLevel::Progress) << "63 %";

	// With all provinces and rulers religion/culture set, only now can we import advisers, which also need religion/culture set.
	// Those advisers coming without such data use the monarch's religion/culture.
	importAdvisers(theConfiguration.getStartDateOption(), sourceWorld.getConversionDate());
	Log(LogLevel::Progress) << "64 %";

	// Rulers with multiple crowns either get PU agreements, or just annex the other crowns.
	resolvePersonalUnions();
	Log(LogLevel::Progress) << "65 %";

	// We're onto the finesse part of conversion now. HRE was shattered in CK2 World and now we're assigning electorates, free
	// cities, and such.
	distributeHRESubtitles(theConfiguration);

	if (sourceWorld.getHRETitle())
	{
		Log(LogLevel::Info) << "-> Marking HRE Title";
		markHRETag(theConfiguration, sourceWorld.getHRETitle()->first);
	}
	Log(LogLevel::Progress) << "66 %";

	// With all religious/cultural matters taken care of, we can now set reforms
	assignAllCountryReforms();
	Log(LogLevel::Progress) << "67 %";

	// Vassalages and tributaries were also set in ck2 world but we have to transcribe those into EU4 agreements.
	auto actualConversionDate = sourceWorld.getConversionDate();
	if (theConfiguration.getStartDateOption() == Configuration::STARTDATE::EU)
		actualConversionDate = date(1444, 11, 11);
	diplomacy.importAgreements(countries, sourceWorld.getDiplomacy(), actualConversionDate);
	Log(LogLevel::Progress) << "68 %";

	// We're distributing permanent claims according to dejure distribution.
	distributeClaims(theConfiguration);

	// We're distributing dead cores according to cultures.
	distributeDeadCores();

	Log(LogLevel::Progress) << "69 %";

	// Now for the final tweaks.
	distributeForts();
	Log(LogLevel::Progress) << "70 %";

	// Tengri
	fixTengri();
	Log(LogLevel::Progress) << "71 %";

	// China
	adjustChina(sourceWorld, theConfiguration.getStartDateOption());
	Log(LogLevel::Progress) << "72 %";

	// Filter dead relationships
	diplomacy.filterDeadRelationships(countries, titleTagMapper.getAllChinas());

	// Check for duplicate country names and rename accordingly
	fixDuplicateNames();

	// Siberia
	siberianQuestion(theConfiguration);
	Log(LogLevel::Progress) << "73 %";

	// African Passes
	africaQuestion();
	Log(LogLevel::Progress) << "74 %";

	// Indian buddhisms
	indianQuestion();
	Log(LogLevel::Progress) << "75 %";

	// And finally, the Dump.
	Log(LogLevel::Info) << "---> The Dump <---";
	modFile.outname = theConfiguration.getOutputName();
	modFile.version = converterVersion.getMaxTarget();
	output(converterVersion, theConfiguration, sourceWorld);
	Log(LogLevel::Info) << "*** Farewell EU4, granting you independence. ***";
}

void EU4::World::indianQuestion()
{
	// countries with capitals in india or persia superregions, need to have their provinces updated to buddhism (from vajrayana),
	// as well as state religion if vajrayana.
	Log(LogLevel::Info) << "-> Resolving the Indian Question";
	auto countryCounter = 0;
	auto provinceCounter = 0;

	for (const auto& country: countries)
	{
		if (!country.second->getTitle().second)
			continue;
		if (country.second->getProvinces().empty())
			continue;

		const auto capitalID = country.second->getCapitalID();
		if (!capitalID)
			continue;

		const auto& capitalSuperRegion = regionMapper->getParentSuperRegionName(capitalID);
		if (!capitalSuperRegion)
			continue;

		if (*capitalSuperRegion == "india_superregion" || *capitalSuperRegion == "persia_superregion")
		{
			// Let's convert to buddhism.
			if (country.second->getReligion() == "vajrayana")
			{
				country.second->setReligion("buddhism");
				country.second->correctRoyaltyToBuddhism();
				++countryCounter;
			}
			for (const auto& province: country.second->getProvinces())
				if (province.second->getReligion() == "vajrayana")
				{
					province.second->setReligion("buddhism");
					++provinceCounter;
				}
		}
	}
	Log(LogLevel::Info) << "-> " << countryCounter << " countries and " << provinceCounter << " provinces resolved.";
}


void EU4::World::scrapeColors(const Configuration& theConfiguration, const CK2::World& sourceWorld)
{
	Log(LogLevel::Info) << "-> Soaking Up Colors";
	auto fileNames = commonItems::GetAllFilesInFolder(theConfiguration.getCK2Path() + "/common/landed_titles/");
	for (const auto& file: fileNames)
	{
		if (file.find(".txt") == std::string::npos)
			continue;
		colorScraper.scrapeColors(theConfiguration.getCK2Path() + "/common/landed_titles/" + file);
	}
	for (const auto& mod: sourceWorld.getMods())
	{
		fileNames = commonItems::GetAllFilesInFolder(mod.path + "/common/landed_titles/");
		if (!fileNames.empty())
			Log(LogLevel::Info) << "\t>> Found some colors in [" << mod.name << "]: " << mod.path;
		for (const auto& file: fileNames)
		{
			if (file.find(".txt") == std::string::npos)
				continue;
			colorScraper.scrapeColors(mod.path + "/common/landed_titles/" + file);
		}
	}
	Log(LogLevel::Info) << ">> " << colorScraper.getColors().size() << " colors soaked up.";
}

void EU4::World::distributeDeadCores()
{
	Log(LogLevel::Info) << "-- Distributing Dead Cores";
	auto counter = 0;

	// Make a register of dead country tags we are allowed to use
	std::set<std::string> deadTags;

	for (const auto& country: countries)
		if (country.second->getProvinces().empty())
			deadTags.insert(country.first);

	// Then, for all provinces, see if their culture has a primary nation, and if we're allowed to insert it.
	for (const auto& province: provinces)
	{
		// Don't touch ROTW
		if (!province.second->getSourceProvince())
			continue;
		// Don't touch stuff we left empty
		if (province.second->getOwner().empty())
			continue;

		const auto& primaryTag = primaryTagMapper.getPrimaryTagForCulture(province.second->getCulture());
		if (primaryTag)
		{
			if (deadTags.count(*primaryTag))
			{
				// We're ok to use this.
				province.second->addCore(*primaryTag);
				++counter;
			}
		}
	}
	Log(LogLevel::Info) << "<> " << counter << " dead cores have been distributed.";
}

void EU4::World::distributeClaims(const Configuration& theConfiguration)
{
	if (theConfiguration.getDejure() == Configuration::DEJURE::DISABLED)
	{
		Log(LogLevel::Info) << ">< Permaclaim distribution disabled by configuration.";
		return;
	}

	Log(LogLevel::Info) << "-- Distributing DeJure Claims";
	auto counter = 0;
	std::map<int, std::set<std::string>> claimsRegister; // ck2 province, eu4 tag claims

	// Mapping all countries with all their claims.
	for (const auto& country: countries)
	{
		if (country.second->getTitle().first.empty())
			continue;
		if (country.second->getProvinces().empty())
			continue; // No claims for dead nations.
		for (const auto& DJprovince: country.second->getTitle().second->getDeJureProvinces())
		{
			claimsRegister[DJprovince.first].insert(country.first);
		}
	}

	// And then rolling through provinces and applying those claims.
	for (const auto& province: provinces)
	{
		if (!province.second->getSourceProvince())
			continue;
		const auto& claimItr = claimsRegister.find(province.second->getSourceProvince()->getID());
		if (claimItr == claimsRegister.end())
			continue;
		for (const auto& tag: claimItr->second)
		{
			if (tag == province.second->getOwner())
				continue;
			// since de jure claims are based on DE JURE land, we're adding it even for PU or vassal land.
			province.second->addPermanentClaim(tag);
			counter++;
		}
	}
	Log(LogLevel::Info) << "<> " << counter << " claims have been distributed.";
}

void EU4::World::fixDuplicateNames()
{
	Log(LogLevel::Info) << "-- Renaming Duplicate Countries";
	auto counter = 0;

	// Iterate through countries and assign their names as keywords. If a name appears more than once, add it to list of pointers
	std::map<const std::string, std::vector<std::shared_ptr<Country>>> nameMap;
	for (const auto& country: countries)
	{
		if (country.second->getProvinces().empty())
			continue;
		const auto& countryLocs = country.second->getLocalizations();
		if (countryLocs.empty())
			continue;
		if (countryLocs.find(country.first) == countryLocs.end())
			continue;
		if (countryLocs.find(country.first)->second.english.empty())
			continue;
		nameMap[countryLocs.find(country.first)->second.english].emplace_back(country.second);
	}

	// Reorder countries in list by development (highest -> lowest)
	for (auto& countryBatch: nameMap)
	{
		std::sort(countryBatch.second.begin(), countryBatch.second.end(), [](auto a, auto b) {
			return a->getDevelopment() > b->getDevelopment();
		});
	}

	// Now we iterate through all batches and sort out the names.
	for (const auto& countryBatch: nameMap)
	{
		// Bail for singletons
		if (countryBatch.second.size() <= 1)
			continue;

		// This is the locblock we're operating on. Should be same for all countries in this batch.
		auto currentBlock = countryBatch.second[0]->getLocalizations().find(countryBatch.second[0]->getTag())->second;

		// Is this a dynastyname? These will follow a bit different rules.
		auto dynastyName = countryBatch.second[0]->getHasDynastyName();

		// Clear out any "Greater" from name.
		auto greaterdropped = false;
		if (currentBlock.english.find("Greater ") != std::string::npos)
		{
			currentBlock.english = currentBlock.english.erase(0, 8); // -Greater
			currentBlock.spanish = currentBlock.spanish.erase(0, 5); // -Gran
			currentBlock.french = currentBlock.french.erase(0, 7);	// -Grande
			currentBlock.german = currentBlock.german.erase(0, 5);	// -Gro�
			greaterdropped = true;
		}

		// Ditto for "Lesser"
		auto lesserdropped = false;
		if (currentBlock.english.find("Lesser ") != std::string::npos)
		{
			currentBlock.english = currentBlock.english.erase(0, 7); // -Lesser
			currentBlock.spanish = currentBlock.spanish.erase(0, 8); // -Peque�a
			currentBlock.french = currentBlock.french.erase(0, 7);	// -Petite
			currentBlock.german = currentBlock.german.erase(0, 6);	// -Klein
			lesserdropped = true;
		}

		// and now let's get to work.

		for (auto i = 0; i < static_cast<int>(countryBatch.second.size()); i++)
		{
			mappers::LocBlock newBlock;
			const auto& actualCountry = countryBatch.second[i];
			const auto& actualTag = actualCountry->getTag();
			const auto& countryLocs = actualCountry->getLocalizations();

			// First we deal with dynastynames.
			if (dynastyName)
			{
				switch (i)
				{
					case 0:
						// First country. Nothing changes (Ottomans).
						break;
					case 5:
						// For the fifth, we do Secret Ottomans.
						newBlock.english = "Secret " + currentBlock.english;
						newBlock.spanish = currentBlock.spanish + " Secreta";
						newBlock.french = currentBlock.french + " Secr�te";
						newBlock.german = "Geheimnis " + currentBlock.german;
						actualCountry->setLocalizations(newBlock);
						break;
					default:
						// Otherwise we're trying to fuse TAG_ADJ and a canonical name for the country. (Ottoman Austria)
						if (countryLocs.count("canonical") && countryLocs.find(actualTag + "_ADJ") != countryLocs.end() &&
							 !countryLocs.find(actualTag + "_ADJ")->second.english.empty())
						{
							// We have the ingredients and can create the name.
							const auto& canonicalBlock = countryLocs.find("canonical")->second;
							const auto& adjBlock = countryLocs.find(actualTag + "_ADJ")->second;
							newBlock.english = adjBlock.english + " " + canonicalBlock.english;
							newBlock.spanish = adjBlock.spanish + " " + canonicalBlock.spanish;
							newBlock.german = adjBlock.german + " " + canonicalBlock.german;
							newBlock.french = adjBlock.french + " " + canonicalBlock.french;
							actualCountry->setLocalizations(newBlock);
							break;
						}
						// If we at least have canonical, we can bail from dynasty names. (just Austria)
						if (countryLocs.count("canonical"))
						{
							newBlock = countryLocs.find("canonical")->second;
							actualCountry->setLocalizations(newBlock);
							break;
						}
						// Else crap. We don't have a canonical name for these fellows. Leave as is.
						break;
				}
			}

			else // These would be regular, non-dynastyname countries.
			{
				switch (i)
				{
					case 0:
						// This is the Greater one, unless it already was the greater one. In that case we diversify.
						if (!greaterdropped)
						{
							newBlock.english = "Greater " + currentBlock.english;
							newBlock.spanish = "Gran " + currentBlock.spanish;
							newBlock.french = "Grande " + currentBlock.french;
							newBlock.german = "Gro� " + currentBlock.german;
						}
						else
						{
							newBlock.english = currentBlock.english + " Major";
							newBlock.spanish = currentBlock.spanish + " Maior";
							newBlock.french = currentBlock.french + " Majeur";
							newBlock.german = currentBlock.german + " Maior";
						}
						actualCountry->setLocalizations(newBlock);
						break;
					case 1:
						// This is Lesser, unless it's Minor.
						if (!lesserdropped)
						{
							newBlock.english = "Lesser " + currentBlock.english;
							newBlock.spanish = "Peque�a " + currentBlock.spanish;
							newBlock.french = "Petite " + currentBlock.french;
							newBlock.german = "Klein " + currentBlock.german;
						}
						else
						{
							newBlock.english = currentBlock.english + " Minor";
							newBlock.spanish = currentBlock.spanish + " Minor";
							newBlock.french = currentBlock.french + " Mineur";
							newBlock.german = currentBlock.german + " Minor";
						}
						actualCountry->setLocalizations(newBlock);
						break;
					case 2:
						// Hello Hither.
						newBlock.english = "Hither " + currentBlock.english;
						newBlock.spanish = "Hither " + currentBlock.english;
						newBlock.french = "Hither " + currentBlock.english;
						newBlock.german = "Hither " + currentBlock.english;
						actualCountry->setLocalizations(newBlock);
						break;
					case 3:
						// Hello Further.
						newBlock.english = "Further " + currentBlock.english;
						newBlock.spanish = "Lejos " + currentBlock.spanish;
						newBlock.french = "Plus " + currentBlock.french;
						newBlock.german = "Ferner " + currentBlock.german;
						actualCountry->setLocalizations(newBlock);
						break;
					case 4:
						// This one's easy.
						newBlock.english = "Secret " + currentBlock.english;
						newBlock.spanish = currentBlock.spanish + " Secreta";
						newBlock.french = currentBlock.french + " Secr�te";
						newBlock.german = "Geheimnis " + currentBlock.german;
						actualCountry->setLocalizations(newBlock);
						break;
					default:
						// Out of ideas. 6 Polands? Don't care any more.
						break;
				}
			}
			counter++;
		}
	}

	Log(LogLevel::Info) << "<> " << counter << " names unduplicated.";
}

void EU4::World::siberianQuestion(const Configuration& theConfiguration)
{
	Log(LogLevel::Info) << "-- Burning Siberia";
	if (theConfiguration.getSiberia() == Configuration::SIBERIA::LEAVE_SIBERIA)
	{
		Log(LogLevel::Info) << ">< Leaving Siberia alone due to Configuration.";
		return;
	}

	auto counter = 0;

	// We're deleting all tags with:
	// * capital is in Siberia
	// * nomad or tribal level
	// * no more than 5 provinces / option
	for (const auto& country: countries)
	{
		if (country.second->getGovernment() != "nomad" && country.second->getGovernment() != "tribal")
			continue;
		if (country.second->getProvinces().empty())
			continue;
		if (!country.second->getCapitalID())
			continue;
		const auto& region = regionMapper->getParentRegionName(country.second->getCapitalID());
		if (!region)
			continue;
		if (region != "west_siberia_region" && region != "east_siberia_region")
			continue;
		if (theConfiguration.getSiberia() == Configuration::SIBERIA::SMALL_SIBERIA && country.second->getProvinces().size() > 5)
			continue;
		if (country.second->getTag() == "CHU" || country.second->getTag() == "HOD" || country.second->getTag() == "CHV" || country.second->getTag() == "KMC")
			continue;


		// All checks done. Let's get deleting.
		for (const auto& province: country.second->getProvinces())
		{
			province.second->sterilize();
		}
		country.second->clearProvinces();
		country.second->clearExcommunicated();
		diplomacy.deleteAgreementsWithTag(country.first);
		++counter;
	}
	Log(LogLevel::Info) << "<> " << counter << " Siberian countries have been removed";
}

void EU4::World::africaQuestion()
{
	Log(LogLevel::Info) << "-- Checking Africa";

	// If a country has a province on both ends of the pass, or if they are in HRE, it stays, otherwise the pass is getting cleared

	for (const auto& pass: africanPassesMapper.getPasses())
	{
		std::vector<int> allProvinces = pass.getEndA();
		allProvinces.insert(allProvinces.end(), pass.getEndB().begin(), pass.getEndB().end());
		allProvinces.insert(allProvinces.end(), pass.getSterilizeProvinces().begin(), pass.getSterilizeProvinces().end());

		// first check the sanity of all provinces.
		auto sanity = true;
		for (const auto& provinceID: allProvinces)
		{
			if (provinces.find(provinceID) == provinces.end())
			{
				Log(LogLevel::Warning) << "Province " << provinceID << " in pass " << pass.getName() << " not found! Skipping!";
				sanity = false;
				break;
			}
			if (!provinces.find(provinceID)->second)
			{
				Log(LogLevel::Warning) << "Province " << provinceID << " in pass " << pass.getName() << " is dangling! Skipping!";
				sanity = false;
				break;
			}
		}
		if (!sanity)
			continue;

		// check owners of pass ends.
		std::set<std::string> ownersA;
		std::set<std::string> ownersB;
		bool hasStart = false;
		bool hasEnd = false;

		for (const auto& provinceID: pass.getEndA())
		{
			if (!provinces.find(provinceID)->second->getOwner().empty())
				ownersA.insert(provinces.find(provinceID)->second->getOwner());
		}
		for (const auto& provinceID: pass.getEndB())
		{
			if (!provinces.find(provinceID)->second->getOwner().empty())
				ownersB.insert(provinces.find(provinceID)->second->getOwner());
		}
		for (const auto& owner: ownersA)
		{
			if (ownersB.contains(owner))
			{
				hasStart = true;
				break;
			}
		}
		if (hasStart)
		{
			Log(LogLevel::Info) << "<> " << pass.getName() << " Pass Colonized.";
			continue; // all under same owner.
		}

		// check HRE
		hasStart = false;

		for (const auto& provinceID: pass.getEndA())
		{
			const auto& country = countries.find(provinces.find(provinceID)->second->getOwner());
			if (country != countries.end() && country->second && country->second->isinHRE())
				hasStart = true;
		}
		for (const auto& provinceID: pass.getEndB())
		{
			const auto& country = countries.find(provinces.find(provinceID)->second->getOwner());
			if (country != countries.end() && country->second && country->second->isinHRE())
				hasEnd = true;
		}
		if (hasStart && hasEnd)
		{
			Log(LogLevel::Info) << "<> " << pass.getName() << " Pass HREified.";
			continue; // all provinces in hre
		}

		// sterilize
		for (const auto& provinceID: pass.getSterilizeProvinces())
			provinces.find(provinceID)->second->sterilize();

		Log(LogLevel::Info) << "<> " << pass.getName() << " Pass Sterilized.";
	}
}

void EU4::World::adjustChina(const CK2::World& sourceWorld, const Configuration::STARTDATE& startDateOption)
{
	// Super Mongolia?
	if (countries.count("MGE") && !countries.find("MGE")->second->getProvinces().empty() && countries.count("KHA"))
	{
		bool isSource = false;
		for (auto province: countries.find("KHA")->second->getProvinces())
		{
			if (province.second->getSourceProvince())
			{
				isSource = true;
				break;
			}
		}
		if (!isSource)
		{
			// Move all Mongolia provinces under Mongol Empire, only happens if Mongolia is seperate from the Mongol Empire
			for (const auto& province: provinces)
			{
				if (province.second->getOwner() == "KHA")
				{
					province.second->setOwner("MGE");
					province.second->setController("MGE");
					province.second->addCore("MGE");
				}
			}

			// Add cores for provinces in China/Manchuria
			provinces.find(4674)->second->addCore("MGE");
			provinces.find(722)->second->addCore("MGE");
			provinces.find(4675)->second->addCore("MGE");
			provinces.find(4672)->second->addCore("MGE");
			provinces.find(702)->second->addCore("MGE");
			provinces.find(701)->second->addCore("MGE");
			provinces.find(4223)->second->addCore("MGE");
		}
	}


	Log(LogLevel::Info) << "-- Invading China";

	std::string ourChinaTag;
	// Do we have a china?

	const auto& china = sourceWorld.getOffmaps().getChina();
	if (!china)
	{
		// No china. Ok, we need to run through china provinces and update ownership from ming to whatever is our chinese default.
		const auto& chinaTag = titleTagMapper.getChinaForTitle("");
		if (!chinaTag)
		{
			Log(LogLevel::Info) << ">< No China in Savegame, no china fallback!";
			return;
		}
		ourChinaTag = *chinaTag;
	}
	else
	{
		const auto& chinaTag = titleTagMapper.getChinaForTitle(china->second->getName());
		if (!chinaTag)
		{
			Log(LogLevel::Info) << ">< Ingame " << china->second->getName() << " china not recognized, no china fallback!";
			return;
		}
		ourChinaTag = *chinaTag;
	}
	const auto& countryItr = countries.find(ourChinaTag);
	if (countryItr == countries.end())
	{
		Log(LogLevel::Warning) << ourChinaTag << " is not loaded at all!";
		return;
	}
	const auto& ourChina = countryItr->second;
	if (!ourChina)
	{
		Log(LogLevel::Warning) << ourChinaTag << " has no loaded definition!";
		return;
	}

	// Move all china provinces under new tag
	for (const auto& chineseProvince: provinceMapper->getOffmapChineseProvinces())
	{
		const auto provinceItr = provinces.find(chineseProvince);
		if (provinceItr == provinces.end())
		{
			Log(LogLevel::Warning) << "Province " << chineseProvince << " is not in fact a valid province.";
			continue;
		}
		provinceItr->second->setOwner(ourChinaTag);
		provinceItr->second->setController(ourChinaTag);
		provinceItr->second->addCore(ourChinaTag);
	}

	celestialEmperorTag = ourChinaTag;
	diplomacy.updateTagsInAgreements("MNG", ourChinaTag);

	// Find western protectorate if possible
	for (const auto& country: countries)
	{
		if (tianxia)
		{
			if (country.second->getTitle().first != "e_china")
				continue;
		}
		else if (country.second->getTitle().first != "e_china_west_governor")
		{
			continue;
		}
		const auto& westernTag = country.first;
		// Move our diplo to China
		diplomacy.updateTagsInAgreements(westernTag, ourChinaTag);
		// Move our provinces to China
		for (const auto& province: country.second->getProvinces())
		{
			province.second->addDiscoveredBy("chinese");
			province.second->setOwner(ourChinaTag);
			province.second->setController(ourChinaTag);
			province.second->addCore(ourChinaTag);
		}
		break;
	}

	// Grab the emperor
	const auto& holder = china->second->getHolder();
	if (holder.second && holder.second->getDynasty().second)
	{
		// We have all data to set emperor.
		Character emperor;
		emperor.birthDate = holder.second->getBirthDate();
		if (holder.second->isFemale())
			emperor.female = true;
		emperor.name = holder.second->getName();
		emperor.dynasty = holder.second->getDynasty().second->getName();
		emperor.adm = std::min((holder.second->getSkills().stewardship + holder.second->getSkills().learning) / 3 + 1, 6);
		emperor.dip = std::min((holder.second->getSkills().diplomacy + holder.second->getSkills().intrigue) / 3 + 1, 6);
		emperor.mil = std::min((holder.second->getSkills().martial + holder.second->getSkills().learning) / 3 + 1, 6);
		std::string baseReligion;
		if (!holder.second->getReligion().empty())
			baseReligion = holder.second->getReligion();
		if (!baseReligion.empty())
		{
			const auto& religionMatch = religionMapper.getEu4ReligionForCk2Religion(baseReligion);
			if (religionMatch)
				emperor.religion = *religionMatch;
			else
				Log(LogLevel::Warning) << "Celestial emperor has no religion from: " << baseReligion;
		}
		else
			Log(LogLevel::Warning) << "Celestial emperor could not determine base religion!";
		std::string baseCulture;
		if (!holder.second->getCulture().empty())
			baseCulture = holder.second->getCulture();
		if (!baseCulture.empty())
		{
			const auto& cultureMatch = cultureMapper.cultureMatch(baseCulture, emperor.religion, 0, ourChinaTag);
			if (cultureMatch)
				emperor.culture = *cultureMatch;
			else
				Log(LogLevel::Warning) << "Celestial emperor has no culture from: " << baseCulture;
		}
		else
			Log(LogLevel::Warning) << "Celestial emperor could not determine base culture!";
		emperor.isSet = true;
		auto actualConversionDate = sourceWorld.getConversionDate();
		if (startDateOption == Configuration::STARTDATE::EU)
			actualConversionDate = date(1444, 11, 11);
		ourChina->setConversionDate(actualConversionDate);
		ourChina->clearHistoryLessons();
		if (!emperor.culture.empty())
			ourChina->addAcceptedCulture(emperor.culture);
		ourChina->setMonarch(emperor);
	}
	else
	{
		Log(LogLevel::Warning) << ">< Celestial emperor has lacking definitions!";
		return;
	}

	// A standalone support for Chinese Caliphates Mod begins here. We import variables from sourceworks defining
	// weights of religions in china (sunni = 40.0, christian = 100.0), and for a total weight of 400 we distribute
	// these religions across chinese provinces. We only do so for offmap provinces as western protectorate provinces
	// may have had their religion already changed.

	auto chineseReligions = sourceWorld.getVars().getChineseReligions();
	if (chineseReligions)
	{
		const auto provinceNum = static_cast<int>(provinceMapper->getOffmapChineseProvinces().size());
		const auto provinceWeight = 400.0 / provinceNum; // religious weight of an individual province.

		for (const auto& chineseProvince: provinceMapper->getOffmapChineseProvinces())
		{
			const auto provinceItr = provinces.find(chineseProvince);
			if (provinceItr == provinces.end())
			{
				continue;
			}
			bool provinceSet = false;
			for (const auto& religiousInfluence: *chineseReligions)
			{
				if (religiousInfluence.second > provinceWeight)
				{
					if (religiousInfluence.second == 0.0)
						continue;
					// is this a valid religion?
					const auto& targetReligion = religionMapper.getEu4ReligionForCk2Religion(religiousInfluence.first);
					if (!targetReligion)
					{
						(*chineseReligions)[religiousInfluence.first] = 0;
						continue;
					}
					provinceItr->second->setReligion(*targetReligion);
					(*chineseReligions)[religiousInfluence.first] -= provinceWeight;
					if ((*chineseReligions)[religiousInfluence.first] < provinceWeight)
						(*chineseReligions)[religiousInfluence.first] = 0;
					provinceSet = true;
					break;
				}
			}
			if (!provinceSet)
				break; // We're done.
		}
	}

	Log(LogLevel::Info) << "<> China successfully Invaded";
}

void EU4::World::fixTengri()
{
	// We need to convert all unmapped EU4 Tengri provinces to Old Tengri (Unreformed)
	Log(LogLevel::Info) << "-- Checking for Reformed Tengri";
	for (const auto& province: provinces)
	{
		if (!province.second->getSourceProvince())
		{
			if (province.second->getReligion() == "tengri_pagan_reformed")
			{
				province.second->setReligion("tengri_pagan");
			}
		}
	}
	for (const auto& country: countries)
	{
		if (country.second->getTitle().first.empty())
		{
			if (country.second->getReligion() == "tengri_pagan_reformed")
			{
				country.second->setReligion("tengri_pagan");
			}
		}
	}
	Log(LogLevel::Info) << ">< Tengri successfully unreformed";
}

void EU4::World::verifyCapitals()
{
	Log(LogLevel::Info) << "-- Verifying All countries Have Capitals";

	auto counter = 0;
	for (const auto& country: countries)
	{
		// POPE is special. Of course. Skip this for pope because he may end up with a capital in new world or something.
		if (country.first == "PAP" || country.first == "FAP")
			continue;
		if (country.second->verifyCapital(*provinceMapper))
			counter++;
	}

	Log(LogLevel::Info) << "<> " << counter << " capitals have been reassigned.";
}

void EU4::World::distributeForts()
{
	Log(LogLevel::Info) << "-- Building Forts";

	// We're doing this only for our new countries. We haven't deleted forts in ROTW.
	// Countries at 4+ provinces get a capital fort. 8+ countries get one fort per area where they own 3+ provinces.

	auto counterCapital = 0;
	auto counterOther = 0;

	for (const auto& country: countries)
	{
		if (country.second->getTitle().first.empty())
			continue;
		if (country.second->getProvinces().size() < 4)
			continue; // To small to afford forts.
		if (!country.second->getCapitalID())
			continue; // Not dealing with broken countries, thank you.
		if (!country.second->getProvinces().count(country.second->getCapitalID()))
		{
			if (country.first != "PAP")
				Log(LogLevel::Warning) << country.first << " has capital province set to " << country.second->getCapitalID() << " but doesn't own it?";
			continue; // this should have been fixed earlier by verifyCapitals! Well... Except for pope.
		}

		const auto& capitalAreaName = regionMapper->getParentAreaName(country.second->getCapitalID());
		if (!capitalAreaName)
			continue; // uh-huh

		const auto& capitalProvince = country.second->getProvinces().find(country.second->getCapitalID());
		capitalProvince->second->buildFort();
		counterCapital++;

		if (country.second->getProvinces().size() < 8)
			continue; // Too small for more forts.

		std::set<std::string> builtAreas = {*capitalAreaName};
		// Now it gets serious. We need a list of areas with 3+ provinces.
		std::map<std::string, int> elegibleAreas;

		for (const auto& province: country.second->getProvinces())
		{
			const auto& areaName = regionMapper->getParentAreaName(province.first);
			if (!areaName)
				continue;
			elegibleAreas[*areaName]++;
		}

		// And we put a fort in every third?
		auto counter = 0;
		for (const auto& province: country.second->getProvinces())
		{
			const auto& areaName = regionMapper->getParentAreaName(province.first);
			if (!areaName)
				continue;
			if (builtAreas.count(*areaName))
				continue;
			if (elegibleAreas[*areaName] <= 2)
				continue;
			if (counter < 2)
			{
				++counter;
				continue;
			}
			counter = 0;

			province.second->buildFort();
			counterOther++;
			builtAreas.insert(*areaName);
		}
	}

	Log(LogLevel::Info) << "<> " << counterCapital << " capital forts and " << counterOther << " other forts have been built.";
}

void EU4::World::alterProvinceDevelopment()
{
	Log(LogLevel::Info) << "-- Scaling Imported provinces";
	// For every 10 buildings in a province we assign a dev point (barony itself counts as 3).
	// We adhere to the distribution key:
	// castle: 2/3 mil 1/3 adm
	// city: dip
	// temple: adm
	// nomad, tribal: mil
	//
	// We're ignoring hospitals at this stage.

	auto totalVanillaDev = 0;
	auto totalCK2Dev = 0;
	auto counter = 0;

	for (const auto& province: provinces)
	{
		if (!province.second->getSourceProvince())
			continue;
		totalVanillaDev += province.second->getDev();
		auto adm = 0.0;
		auto dip = 0.0;
		auto mil = 0.0;
		const auto& baronies = province.second->getSourceProvince()->getBaronies();
		for (const auto& barony: baronies)
		{
			if (barony.second->getType().empty())
				continue;
			const auto buildingNumber = static_cast<double>(barony.second->getBuildingCount());
			const auto baronyDev = devWeightsMapper.getDevFromBarony() + devWeightsMapper.getDevFromBuilding() * buildingNumber;
			if (barony.second->getType() == "tribal" || barony.second->getType() == "nomad")
			{
				mil += baronyDev;
			}
			else if (barony.second->getType() == "city")
			{
				dip += baronyDev;
			}
			else if (barony.second->getType() == "temple")
			{
				adm += baronyDev;
			}
			else if (barony.second->getType() == "castle")
			{
				adm += baronyDev * 1 / 3; // third to adm
				mil += baronyDev * 2 / 3; // two thirds to mil
			}
		}
		province.second->setAdm(std::max(static_cast<int>(std::lround(adm)), 1));
		province.second->setDip(std::max(static_cast<int>(std::lround(dip)), 1));
		province.second->setMil(std::max(static_cast<int>(std::lround(mil)), 1));
		counter++;
		totalCK2Dev += province.second->getDev();
	}

	Log(LogLevel::Info) << "<> " << counter << " provinces scaled: " << totalCK2Dev << " development imported (vanilla had " << totalVanillaDev << ").";
}

void EU4::World::importAdvisers(Configuration::STARTDATE startDateOption, date theConversionDate)
{
	Log(LogLevel::Info) << "-> Importing Advisers";
	auto counter = 0;
	for (const auto& country: countries)
	{
		country.second->initializeAdvisers(religionMapper, cultureMapper, startDateOption, theConversionDate);
		counter += static_cast<int>(country.second->getAdvisers().size());
	}
	Log(LogLevel::Info) << "<> Imported " << counter << " advisers.";
}


void EU4::World::resolvePersonalUnions()
{
	const std::set<std::string> elegibleReligions = {"catholic",
		 "orthodox",
		 "nestorian",
		 "coptic",
		 "cathar",
		 "fraticelli",
		 "waldensian",
		 "lollard",
		 "bogomilist",
		 "monothelite",
		 "iconoclast",
		 "messalian",
		 "paulician",
		 "monophysite"};

	std::map<int, std::map<std::string, std::shared_ptr<Country>>> holderTitles;
	std::map<int, std::pair<std::string, std::shared_ptr<Country>>> holderPrimaryTitle;
	std::map<int, std::shared_ptr<CK2::Character>> relevantHolders;

	// We're filling the registry first.
	for (const auto& country: countries)
	{
		if (country.second->getTitle().first.empty())
			continue;
		if (!country.second->getTitle().second->getHolder().first)
			continue;
		// we have a holder.
		const auto& holder = country.second->getTitle().second->getHolder();
		holderTitles[holder.first].insert(country);
		relevantHolders.insert(holder);
		// does he have a primary title?
		if (!holder.second->getPrimaryTitle().first.empty())
		{
			if (!holder.second->getPrimaryTitle().second->getTitle().second->getEU4Tag().first.empty())
			{
				holderPrimaryTitle[holder.first] = holder.second->getPrimaryTitle().second->getTitle().second->getEU4Tag();
			}
		}
	}

	// Now let's see what we have.
	for (const auto& holderTitle: holderTitles)
	{
		if (holderTitle.second.size() <= 1)
			continue;

		// multiple crowns. What's our primary?
		auto primaryItr = holderPrimaryTitle.find(holderTitle.first);
		std::pair<std::string, std::shared_ptr<Country>> primaryTitle;
		if (primaryItr == holderPrimaryTitle.end() || primaryItr->second.second->getProvinces().empty())
		{
			// We need to find another primary title.
			auto foundPrimary = false;
			// First check if we can find PAP or FAP or some special flag
			for (const auto& title: holderTitle.second)
			{
				if (title.first == "PAP" || title.first == "FAP" || title.second->isHREEmperor() || title.second->isHREElector())
				{
					primaryTitle = std::pair(title.first, title.second);
					foundPrimary = true;
					break;
				}
			}
			// If no popes or specials pick first one.
			if (!foundPrimary)
				for (const auto& title: holderTitle.second)
				{
					if (!title.second->getProvinces().empty())
					{
						primaryTitle = std::pair(title.first, title.second);
						foundPrimary = true;
						break;
					}
				}
			if (!foundPrimary)
				continue; // no helping this fellow.
		}
		else
		{
			primaryTitle = primaryItr->second;

			// That's lovely, but is this the special snowflake THE POPE? Does he hold PAP/FAP as secondary?
			for (const auto& title: holderTitle.second)
			{
				if (title.first == "PAP" || title.first == "FAP")
				{
					primaryTitle = std::pair(title.first, title.second);
					break;
				}
			}
		}

		// religion
		const auto& religion = primaryTitle.second->getReligion();
		auto heathen = false;
		if (!elegibleReligions.count(religion))
			heathen = true;

		// We now have a holder, his primary, and religion. Let's resolve multiple crowns.
		if (!heathen && primaryTitle.second->getGovernment() == "monarchy")
		{
			auto unionCount = 0;
			for (const auto& title: holderTitle.second)
			{
				if (title.first == primaryTitle.first)
					continue;
				if (title.second->getProvinces().empty())
					continue;
				// Craft a relation. Going up to a max of 3 unions.
				if (unionCount <= 2)
				{
					diplomacy.addAgreement(std::make_shared<Agreement>(primaryTitle.first, title.first, "union", primaryTitle.second->getConversionDate()));
					++unionCount;
				}
				else
				{
					// too many unions.
					primaryTitle.second->annexCountry(title);
					if (primaryTitle.second->isHREEmperor() && emperorTag != primaryTitle.first)
						emperorTag = primaryTitle.first;
				}
			}
		}
		else
		{
			// heathens annex straight up.
			for (const auto& title: holderTitle.second)
			{
				if (title.first == primaryTitle.first)
					continue;
				if (title.second->getProvinces().empty())
					continue;
				// Yum.
				primaryTitle.second->annexCountry(title);
				if (primaryTitle.first == "PAP" || primaryTitle.first == "FAP")
					Log(LogLevel::Debug) << primaryTitle.first << " is annexing " << title.first;
				if (primaryTitle.second->isHREEmperor() && emperorTag != primaryTitle.first)
					emperorTag = primaryTitle.first;
			}
		}
	}
}

void EU4::World::markHRETag(const Configuration& theConfiguration, const std::string& hreTitleName)
{
	if (theConfiguration.getHRE() == Configuration::I_AM_HRE::NONE)
		return;
	if (hreTitleName.empty())
		return;
	const auto hreTag = titleTagMapper.getTagForTitle(hreTitleName);
	if (!hreTag)
	{
		Log(LogLevel::Warning) << "No known tag for CK2's HRE Title: " << hreTitleName << "! Add it to tag_mappings.txt!";
		return;
	}
	actualHRETag = *hreTag;
	if (actualHRETag == "HLR")
		actualHRETag = "HRE";
	Log(LogLevel::Info) << "<> Marked " << actualHRETag << " as HRE tag.";
}

void EU4::World::distributeHRESubtitles(const Configuration& theConfiguration)
{
	if (theConfiguration.getHRE() == Configuration::I_AM_HRE::NONE)
		return;
	Log(LogLevel::Info) << "-> Locating Emperor";
	// Emperor may or may not be set.
	for (const auto& country: countries)
		if (country.second->isHREEmperor())
		{
			emperorTag = country.first;
			Log(LogLevel::Info) << "<> Emperor is " << emperorTag << " (" << country.second->getTitle().first << ", " << country.second->getProvinces().size()
									  << " provinces)";
			break;
		}
	if (!emperorTag.empty())
	{
		setFreeCities();
		setElectors();
	}
	else
		Log(LogLevel::Info) << "<> Emperor could not be found, no HRE mechanics.";
}

void EU4::World::setElectors()
{
	Log(LogLevel::Info) << "-> Setting Electors";
	std::vector<std::pair<int, std::shared_ptr<Country>>> bishops;	  // piety-tag
	std::vector<std::pair<int, std::shared_ptr<Country>>> duchies;	  // dev-tag
	std::vector<std::pair<int, std::shared_ptr<Country>>> republics; // dev-tag
	std::vector<std::shared_ptr<Country>> electors;
	int electorBishops = 0;
	int electorRepublics = 0;
	int electorDuchies = 0;

	// We need to be careful about papacy and orthodox holders
	for (const auto& country: countries)
	{
		if (country.second->isinHRE())
		{
			if (country.second->getProvinces().empty())
				continue;
			const auto& holder = country.second->getTitle().second->getHolder();
			if (country.first == "PAP" || holder.second->getPrimaryTitle().first == "k_orthodox")
			{
				// override to always be elector
				electors.emplace_back(country.second);
				electorBishops++;
				continue;
			}
			// Let's shove all hre members into appropriate categories.
			if (country.second->getGovernment() == "theocracy")
			{
				if (country.second->getTitle().second->isElector())
				{
					electorBishops++;
					electors.emplace_back(country.second);
				}
				else
				{
					bishops.emplace_back(std::pair(lround(holder.second->getPiety()), country.second));
				}
			}
			else if (country.second->getGovernment() == "monarchy")
			{
				if (country.second->getTitle().second->isElector())
				{
					electorDuchies++;
					electors.emplace_back(country.second);
				}
				else
				{
					// skip juniors.
					if (diplomacy.isCountryJunior(country.first))
						continue;
					duchies.emplace_back(std::pair(country.second->getDevelopment(), country.second));
				}
			}
			else if (country.second->getGovernment() == "republic")
			{
				// No free cities, thank you!
				if (country.second->getGovernmentReforms().count("free_city"))
					continue;
				if (country.second->getTitle().second->isElector())
				{
					electorRepublics++;
					electors.emplace_back(country.second);
				}
				else
				{
					republics.emplace_back(std::pair(country.second->getDevelopment(), country.second));
				}
			} // skipping tribal and similar.
		}
	}

	std::sort(bishops.rbegin(), bishops.rend());
	std::sort(duchies.rbegin(), duchies.rend());
	std::sort(republics.rbegin(), republics.rend());

	for (const auto& bishop: bishops)
	{
		if (electors.size() >= 7 || electorBishops >= 3)
			break;
		electors.emplace_back(bishop.second);
		electorBishops++;
	}
	for (const auto& republic: republics)
	{
		if (electors.size() >= 7 || electorBishops + electorRepublics >= 3)
			break;
		electors.emplace_back(republic.second);
		electorRepublics++;
	}
	for (const auto& duchy: duchies)
	{
		if (electors.size() >= 7)
			break;
		electors.emplace_back(duchy.second);
	}

	for (const auto& elector: electors)
	{
		elector->setElector();
		Log(LogLevel::Info) << "\t- Electorate set: " << elector->getTag() << " (from " << elector->getTitle().first << ")";
	}
	Log(LogLevel::Info) << "<> There are " << electors.size() << " electors recognized.";
}

void EU4::World::setFreeCities()
{
	Log(LogLevel::Info) << "-> Setting Free Cities";
	// How many free cities do we already have?
	auto freeCityNum = 0;
	for (const auto& country: countries)
	{
		if (country.second->isinHRE() && country.second->getGovernment() == "republic" && country.second->getProvinces().size() == 1 &&
			 country.second->getTitle().second->getGeneratedLiege().first.empty() && !country.second->getTitle().second->isElector() && freeCityNum < 12)
		{
			country.second->overrideReforms("free_city");
			country.second->setMercantilism(25);
			++freeCityNum;
		}
	}
	// Can we turn some minors into free cities?
	if (freeCityNum < 12)
	{
		for (const auto& country: countries)
		{
			if (country.second->isinHRE() && country.second->getGovernment() != "republic" && !country.second->isHREEmperor() &&
				 country.second->getGovernmentReforms().empty() && country.second->getProvinces().size() == 1 && !diplomacy.isCountryJunior(country.first) &&
				 country.second->getTitle().second->getGeneratedLiege().first.empty() && !country.second->getTitle().second->isElector() && freeCityNum < 12)
			{
				if (country.first == "HAB")
					continue; // For Iohannes who is sensitive about Austria.
				// GovernmentReforms being empty ensures we're not converting special governments and targeted tags into free cities.
				country.second->overrideReforms("free_city");
				country.second->setGovernment("republic");
				country.second->setMercantilism(25);
				++freeCityNum;
			}
		}
	}
	Log(LogLevel::Info) << "<> There are " << freeCityNum << " free cities.";
}

void EU4::World::linkProvincesToCountries()
{
	// Some of the provinces have linked countries, but new world won't. We need to insert links both ways there.
	for (const auto& province: provinces)
	{
		if (province.second->getOwner().empty())
			continue; // this is the uncolonized case
		const auto& countryItr = countries.find(province.second->getOwner());
		if (countryItr != countries.end())
		{
			// registering owner in province
			if (province.second->getTagCountry().first.empty())
				province.second->registerTagCountry(std::pair(countryItr->first, countryItr->second));
			// registering province in owner.
			countryItr->second->registerProvince(std::pair(province.first, province.second));
		}
		else
		{
			Log(LogLevel::Warning) << "Province " << province.first << " owner " << province.second->getOwner() << " has no country!";
		}
	}

	// 1210 - Dawaro needs to be given to Adal (Since Ethiopia could be a country)
	if (provinces.find(1210) != provinces.end() && provinces.find(1210)->second)
	{
		provinces.find(1210)->second->sterilize();
		provinces.find(1210)->second->addCore("ADA");
		provinces.find(1210)->second->setOwner("ADA");
		provinces.find(1210)->second->setController("ADA");
	}
}

template <typename KeyType, typename ValueType> std::pair<KeyType, ValueType> get_max(const std::map<KeyType, ValueType>& x)
{
	using pairtype = std::pair<KeyType, ValueType>;
	return *std::max_element(x.begin(), x.end(), [](const pairtype& p1, const pairtype& p2) {
		return p1.second < p2.second;
	});
}

void EU4::World::verifyReligionsAndCultures()
{
	// We are checking every country if it lacks primary religion and culture. This is an issue for hordeland mainly.
	// For those lacking setups, we'll do a provincial census and inherit those values.
	for (const auto& country: countries)
	{
		// It's possible to get non-christian countries excommunicated through broken setups. Let's clear those immediately.
		if (country.second->isExcommunicated())
		{
			const auto& religion = country.second->getReligion();
			if (religion != "catholic" || religion != "fraticelli")
				country.second->clearExcommunicated();
		}
		// And then proceed on checking the missing boxes.
		if (!country.second->getReligion().empty() && !country.second->getPrimaryCulture().empty() && !country.second->getTechGroup().empty() &&
			 !country.second->getGFX().empty())
			continue;
		if (country.second->getProvinces().empty())
			continue; // No point.

		std::map<std::string, int> religiousCensus;
		std::map<std::string, int> culturalCensus;
		for (const auto& province: country.second->getProvinces())
		{
			if (province.second->getReligion().empty())
			{
				Log(LogLevel::Warning) << "Province " << province.first << " has no religion set!";
				continue;
			}
			if (province.second->getCulture().empty())
			{
				Log(LogLevel::Warning) << "Province " << province.first << " has no culture set!";
				continue;
			}
			religiousCensus[province.second->getReligion()] += 1;
			culturalCensus[province.second->getCulture()] += 1;
		}
		if (country.second->getPrimaryCulture().empty())
		{
			auto max = get_max(culturalCensus);
			Log(LogLevel::Warning) << country.first << " overriding blank culture with: " << max.first;
			country.second->setPrimaryCulture(max.first);
		}
		if (country.second->getMajorityReligion().empty())
		{
			auto max = get_max(religiousCensus);
			Log(LogLevel::Info) << country.first << "'s majority religion is: " << max.first;
			country.second->setMajorityReligion(max.first);
		}
		if (country.second->getReligion().empty())
		{
			auto max = get_max(religiousCensus);
			Log(LogLevel::Warning) << country.first << " overriding blank religion with: " << max.first;
			country.second->setReligion(max.first);
		}
		if (country.second->getTechGroup().empty())
		{
			const auto& techMatch = cultureMapper.getTechGroup(country.second->getPrimaryCulture());
			if (techMatch)
			{
				Log(LogLevel::Warning) << country.first << " overriding blank tech group with: " << *techMatch;
				country.second->setGFX(*techMatch);
			}
			else
			{
				country.second->setGFX("western");
				Log(LogLevel::Warning) << country.first << " could not determine technological group, substituting western!";
			}
		}
		if (country.second->getGFX().empty())
		{
			const auto& gfxMatch = cultureMapper.getGFX(country.second->getPrimaryCulture());
			if (gfxMatch)
			{
				Log(LogLevel::Warning) << country.first << " overriding blank gfx with: " << *gfxMatch;
				country.second->setTechGroup(*gfxMatch);
			}
			else
			{
				country.second->setTechGroup("westerngfx");
				Log(LogLevel::Warning) << country.first << " could not determine GFX, substituting westerngfx!";
			}
		}
	}
}

void EU4::World::importVanillaProvinces(const std::string& eu4Path, bool invasion)
{
	Log(LogLevel::Info) << "-> Importing Vanilla Provinces";
	// ---- Loading history/provinces
	auto fileNames = commonItems::GetAllFilesInFolder(eu4Path + "/history/provinces/");
	for (const auto& fileName: fileNames)
	{
		if (fileName.find(".txt") == std::string::npos)
			continue;
		if (fileName.starts_with('~'))
			continue;
		try
		{
			const auto id = std::stoi(fileName);
			auto newProvince = std::make_shared<Province>(id, eu4Path + "/history/provinces/" + fileName);
			if (provinces.count(id))
			{
				Log(LogLevel::Warning) << "Vanilla province duplication - " << id << " already loaded! Overwriting.";
				provinces[id] = newProvince;
			}
			else
				provinces.insert(std::pair(id, newProvince));
		}
		catch (std::exception& e)
		{
			Log(LogLevel::Warning) << "Invalid province filename: " << eu4Path << "/history/provinces/" << fileName << " : " << e.what();
		}
	}
	Log(LogLevel::Info) << ">> Loaded " << provinces.size() << " province definitions.";
	if (invasion)
	{
		fileNames = commonItems::GetAllFilesInFolder("configurables/sunset/history/provinces/");
		for (const auto& fileName: fileNames)
		{
			if (fileName.find(".txt") == std::string::npos)
				continue;
			auto id = std::stoi(fileName);
			const auto& provinceItr = provinces.find(id);
			if (provinceItr != provinces.end())
				provinceItr->second->updateWith("configurables/sunset/history/provinces/" + fileName);
		}
	}
}

void EU4::World::importCK2Countries(Configuration::STARTDATE startDateOption, const CK2::World& sourceWorld)
{
	Log(LogLevel::Info) << "-> Importing CK2 Countries";

	// countries holds all tags imported from EU4. We'll now overwrite some and
	// add new ones from ck2 titles.
	for (const auto& title: sourceWorld.getIndepTitles())
	{
		if (title.first.find("e_") != 0)
			continue;
		importCK2Country(title, startDateOption, sourceWorld);
	}
	for (const auto& title: sourceWorld.getIndepTitles())
	{
		if (title.first.find("k_") != 0)
			continue;
		importCK2Country(title, startDateOption, sourceWorld);
	}
	for (const auto& title: sourceWorld.getIndepTitles())
	{
		if (title.first.find("d_") != 0)
			continue;
		importCK2Country(title, startDateOption, sourceWorld);
	}
	for (const auto& title: sourceWorld.getIndepTitles())
	{
		if (title.first.find("c_") != 0)
			continue;
		importCK2Country(title, startDateOption, sourceWorld);
	}
	Log(LogLevel::Info) << ">> " << countries.size() << " total countries recognized.";
}

void EU4::World::importCK2Country(const std::pair<std::string, std::shared_ptr<CK2::Title>>& title,
	 Configuration::STARTDATE startDateOption,
	 const CK2::World& sourceWorld)
{
	// Grabbing the capital, if possible
	int eu4CapitalID = 0;
	const auto& ck2CapitalID = title.second->getHolder().second->getCapitalProvince().first;
	if (ck2CapitalID)
	{
		const auto& capitalMatch = provinceMapper->getEU4ProvinceNumbers(ck2CapitalID);
		if (!capitalMatch.empty())
			eu4CapitalID = *capitalMatch.begin();
	}

	// Mapping the title to a tag
	// The Pope is Special! This is land /owned by/ a pope, but might be e_france or k_jerusalem.
	// First title will map to PAP/FAP and others will merge into it.
	std::optional<std::string> tag;
	if (title.second->isThePope())
	{
		tag = titleTagMapper.getTagForTitle("The Pope", title.second->getBaseTitle().first, eu4CapitalID);
	}
	else if (title.second->isTheFraticelliPope())
	{
		tag = titleTagMapper.getTagForTitle("The Fraticelli Pope", title.second->getBaseTitle().first, eu4CapitalID);
	}
	else
	{
		tag = titleTagMapper.getTagForTitle(title.first, title.second->getBaseTitle().first, eu4CapitalID);
	}
	if (!tag)
		throw std::runtime_error("Title " + title.first + " could not be mapped!");

	// Locating appropriate existing country
	const auto& countryItr = countries.find(*tag);
	if (countryItr != countries.end())
	{
		countryItr->second->initializeFromTitle(*tag,
			 title.second,
			 governmentsMapper,
			 religionMapper,
			 cultureMapper,
			 *provinceMapper,
			 colorScraper,
			 localizationMapper,
			 rulerPersonalitiesMapper,
			 startDateOption,
			 sourceWorld.getConversionDate());
		title.second->registerEU4Tag(std::pair(*tag, countryItr->second));
	}
	else
	{
		// Otherwise create the country
		auto newCountry = std::make_shared<Country>();
		newCountry->initializeFromTitle(*tag,
			 title.second,
			 governmentsMapper,
			 religionMapper,
			 cultureMapper,
			 *provinceMapper,
			 colorScraper,
			 localizationMapper,
			 rulerPersonalitiesMapper,
			 startDateOption,
			 sourceWorld.getConversionDate());
		title.second->registerEU4Tag(std::pair(*tag, newCountry));
		countries.insert(std::pair(*tag, newCountry));
	}
}

void EU4::World::importCK2Provinces(const CK2::World& sourceWorld)
{
	Log(LogLevel::Info) << "-> Importing CK2 Provinces";

	auto counter = 0;
	// CK2 provinces map to a subset of eu4 provinces. We'll only rewrite those we are responsible for.
	for (const auto& province: provinces)
	{
		const auto& ck2Provinces = provinceMapper->getCK2ProvinceNumbers(province.first);
		// Provinces we're not affecting will not be in this list.
		if (ck2Provinces.empty())
			continue;
		// Next, we find what province to use as its initializing source.
		const auto& sourceProvince = determineProvinceSource(ck2Provinces, sourceWorld);
		if (!sourceProvince)
		{
			continue; // MISMAP, or simply have mod provinces loaded we're not using.
		}
		if (sourceProvince->first == -1)
		{
			province.second->sterilize(); // sterilizing wastelands
		}
		else
		{
			province.second->initializeFromCK2(sourceProvince->second, cultureMapper, religionMapper);
		}
		// And finally, initialize it.
		counter++;
	}
	Log(LogLevel::Info) << ">> " << sourceWorld.getProvinces().size() << " CK2 provinces imported into " << counter << " EU4 provinces.";
}

void EU4::World::assignAllCountryReforms()
{
	for (const auto& country: countries)
	{
		if (country.second->getTitle().first.empty() || !country.second->getGovernmentReforms().empty())
			continue;
		country.second->assignReforms(regionMapper);
	}
}

void EU4::World::importVanillaCountries(const std::string& eu4Path, bool invasion)
{
	Log(LogLevel::Info) << "-> Importing Vanilla Countries";
	// ---- Loading common/countries/
	std::ifstream eu4CountriesFile(fs::u8path(eu4Path + "/common/country_tags/00_countries.txt"));
	if (!eu4CountriesFile.is_open())
		throw std::runtime_error("Could not open " + eu4Path + "/common/country_tags/00_countries.txt!");
	loadCountriesFromSource(eu4CountriesFile, eu4Path, true);
	eu4CountriesFile.close();
	if (commonItems::DoesFolderExist("blankMod/output/common/country_tags/"))
	{
		auto fileNames = commonItems::GetAllFilesInFolder("blankMod/output/common/country_tags/");
		for (const auto& file: fileNames)
		{
			std::ifstream blankCountriesFile(fs::u8path("blankMod/output/common/country_tags/" + file));
			if (!blankCountriesFile.is_open())
				throw std::runtime_error("Could not open blankMod/output/common/country_tags/" + file + "!");
			loadCountriesFromSource(blankCountriesFile, "blankMod/output/", false);
			blankCountriesFile.close();
		}
	}

	if (invasion)
	{
		std::ifstream sunset(fs::u8path("configurables/sunset/common/country_tags/zz_countries.txt"));
		if (!sunset.is_open())
			throw std::runtime_error("Could not open configurables/sunset/common/country_tags/zz_countries.txt!");
		loadCountriesFromSource(sunset, "configurables/sunset/", true);
		sunset.close();
	}

	Log(LogLevel::Info) << ">> Loaded " << countries.size() << " countries.";

	Log(LogLevel::Info) << "-> Importing Vanilla Country History";
	// ---- Loading history/countries/
	auto fileNames = commonItems::GetAllFilesInFolder(eu4Path + "/history/countries/");
	for (const auto& fileName: fileNames)
	{
		auto tag = fileName.substr(0, 3);
		countries[tag]->loadHistory(eu4Path + "/history/countries/" + fileName);
	}
	// Now our special tags.
	fileNames = commonItems::GetAllFilesInFolder("blankMod/output/history/countries/");
	for (const auto& fileName: fileNames)
	{
		auto tag = fileName.substr(0, 3);
		countries[tag]->loadHistory("blankMod/output/history/countries/" + fileName);
	}
	if (invasion)
	{
		fileNames = commonItems::GetAllFilesInFolder("configurables/sunset/history/countries/");
		for (const auto& fileName: fileNames)
		{
			auto tag = fileName.substr(0, 3);
			countries[tag]->setSunsetCountry(true);
			countries[tag]->clearHistoryLessons();
			countries[tag]->loadHistory("configurables/sunset/history/countries/" + fileName);
		}
	}
	Log(LogLevel::Info) << ">> Loaded " << fileNames.size() << " history files.";
}

void EU4::World::loadCountriesFromSource(std::istream& theStream, const std::string& sourcePath, bool isVanillaSource)
{
	while (!theStream.eof())
	{
		std::string line;
		getline(theStream, line);

		if (line[0] == '#' || line.length() < 4)
			continue;
		auto tag = line.substr(0, 3);

		// All file paths are in quotes. The ones outside are commented, so we can use those as markers.
		auto quoteLoc = line.find_first_of('\"');
		auto countryLine = line.substr(quoteLoc + 1, line.length());
		quoteLoc = countryLine.find_last_of('\"');
		countryLine = countryLine.substr(0, quoteLoc);
		const auto filePath = sourcePath + "/common/" + countryLine;

		// We're soaking up all vanilla countries with all current definitions.
		const auto newCountry = std::make_shared<Country>(tag, filePath);
		if (countries.count(tag))
			countries[tag] = newCountry; // Overriding vanilla EU4 with our definitions.
		else
			countries.insert(std::make_pair(tag, newCountry));
		if (!isVanillaSource)
			specialCountryTags.insert(tag);
	}
}


std::optional<std::pair<int, std::shared_ptr<CK2::Province>>> EU4::World::determineProvinceSource(const std::vector<int>& ck2ProvinceNumbers,
	 const CK2::World& sourceWorld) const
{
	// determine ownership by province development.
	std::map<std::string, std::vector<std::shared_ptr<CK2::Province>>> theClaims; // title, offered province sources
	std::map<std::string, int> theShares;														// title, development
	std::string winner;
	auto maxDev = -1;

	for (auto ck2ProvinceID: ck2ProvinceNumbers)
	{
		const auto& ck2province = sourceWorld.getProvinces().find(ck2ProvinceID);
		if (ck2province == sourceWorld.getProvinces().end())
		{
			continue; // Broken mapping, or loaded a mod changing provinces without using it.
		}
		auto ownerTitle = ck2province->second->getTitle().first;
		if (ownerTitle.empty())
		{
			// This is a wasteland. It means we must blank the eu4 province, no questions asked!
			return std::pair(-1, nullptr);
		}
		theClaims[ownerTitle].push_back(ck2province->second);
		theShares[ownerTitle] = lround(ck2province->second->getBuildingWeight());

		// While at it, is this province especially important? Enough so we'd sidestep regular rules?
		// Check for capital provinces
		if (ck2province->second->getTitle().second->getHolder().second->getCapitalProvince().first == ck2province->first)
		{
			// This is the someone's capital, don't assign it away if unnecessary.
			theShares[ownerTitle] += 200; // Dev can go up to 300+, so yes, assign it away if someone has overbuilt a nearby province.
		}
		// Check for a wonder. For multiple wonders, sorry, only last one will prevail.
		if (ck2province->second->getWonder())
		{
			// This is the someone's wonder province.
			theShares[ownerTitle] += 500;
		}
		// Check for HRE emperor
		if (ck2province->second->getTitle().second->isHREEmperor())
		{
			const auto& emperor = ck2province->second->getTitle().second->getHolder().second;
			if (emperor->getCapitalProvince().first == ck2province->first)
			{
				// This is the empire capital, never assign it away.
				theShares[ownerTitle] += 999;
			}
		}
	}
	// Let's see who the lucky winner is.
	for (const auto& share: theShares)
	{
		if (share.second > maxDev)
		{
			winner = share.first;
			maxDev = share.second;
		}
	}
	if (winner.empty())
	{
		return std::nullopt;
	}

	// Now that we have a winning title, let's find its largest province to use as a source.
	maxDev = -1; // We can have winning provinces with weight = 0;

	std::pair<int, std::shared_ptr<CK2::Province>> toReturn;
	for (const auto& province: theClaims[winner])
	{
		auto provinceWeight = province->getBuildingWeight();
		if (province->getWonder())
			provinceWeight += 500;
		if (province->getTitle().second->getHolder().second->getCapitalProvince().first == province->getID())
			provinceWeight += 200;
		if (province->getTitle().second->isHREEmperor() && province->getTitle().second->getHolder().second->getCapitalProvince().first == province->getID())
			provinceWeight += 999;

		if (provinceWeight > maxDev)
		{
			toReturn.first = province->getID();
			toReturn.second = province;
			maxDev = provinceWeight;
		}
	}
	if (!toReturn.first || !toReturn.second)
	{
		return std::nullopt;
	}
	return std::move(toReturn);
}
