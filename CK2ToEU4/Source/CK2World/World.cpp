#include "World.h"
#include "../Common/Version.h"
#include "Log.h"
#include "OSCompatibilityLayer.h"
#include "ParserHelpers.h"
#include <ZipFile.h>
#include <filesystem>
#include "Date.h"
#include <fstream>
#include "../Configuration/Configuration.h"
#include "../Common/CommonFunctions.h"
#include "Titles/Title.h"
#include "Titles/Liege.h"

namespace fs = std::filesystem;

CK2::World::World(std::shared_ptr<Configuration> theConfiguration)
{
	LOG(LogLevel::Info) << "*** Hello CK2, Deus Vult! ***";
	registerKeyword("CK2txt", [](const std::string& unused, std::istream& theStream) {});
	registerKeyword("date", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString dateString(theStream);
		endDate = date(dateString.getString());
		});
	registerKeyword("start_date", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString startDateString(theStream);
		startDate = date(startDateString.getString());
		});
	registerKeyword("version", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString versionString(theStream);			
		CK2Version = Version(versionString.getString());
		Log(LogLevel::Info) << "Savegame version: " << versionString.getString();
		});
	registerKeyword("provinces", [this](const std::string& unused, std::istream& theStream) {
		LOG(LogLevel::Info) << "-> Loading Provinces";
		provinces = Provinces(theStream);
		LOG(LogLevel::Info) << ">> Loaded " << provinces.getProvinces().size() << " provinces.";
		});
	registerKeyword("character", [this](const std::string& unused, std::istream& theStream) {
		LOG(LogLevel::Info) << "-> Loading Characters";
		characters = Characters(theStream);
		LOG(LogLevel::Info) << ">> Loaded " << characters.getCharacters().size() << " characters.";
		});
	registerKeyword("title", [this](const std::string& unused, std::istream& theStream) {
		LOG(LogLevel::Info) << "-> Loading Titles";
		titles = Titles(theStream);
		LOG(LogLevel::Info) << ">> Loaded " << titles.getTitles().size() << " titles.";
		});
	registerKeyword("dynasties", [this](const std::string& unused, std::istream& theStream) {
		LOG(LogLevel::Info) << "-> Loading Dynasties";
		dynasties = Dynasties(theStream);
		LOG(LogLevel::Info) << ">> Loaded " << dynasties.getDynasties().size() << " dynasties.";
		});
	registerKeyword("dyn_title", [this](const std::string& unused, std::istream& theStream) {
		const auto dynTitle = Liege(theStream);
		dynamicTitles.insert(std::pair(dynTitle.getTitle().first, dynTitle));
		});

	registerRegex("[A-Za-z0-9\\_]+", commonItems::ignoreItem);

	LOG(LogLevel::Info) << "-> Verifying CK2 save.";
	verifySave(theConfiguration->getSaveGamePath());

	LOG(LogLevel::Info) << "-> Importing CK2 save.";
	if (!saveGame.compressed)
	{
		std::ifstream inBinary(fs::u8path(theConfiguration->getSaveGamePath()), std::ios::binary);
		if (!inBinary.is_open())
		{
			LOG(LogLevel::Error) << "Could not open " << theConfiguration->getSaveGamePath() << " for parsing.";
			throw std::runtime_error("Could not open " + theConfiguration->getSaveGamePath() + " for parsing.");
		}
		std::stringstream inStream;
		inStream << inBinary.rdbuf();
		saveGame.gamestate = inStream.str();
	}

	auto gameState = std::istringstream(saveGame.gamestate);
	parseStream(gameState);
	clearRegisteredKeywords();
	LOG(LogLevel::Info) << ">> Loaded " << dynamicTitles.size() << " dynamic titles.";	
	LOG(LogLevel::Info) << "-> Importing Province Titles";
	provinceTitleMapper.loadProvinces(theConfiguration->getCK2Path());
	

	LOG(LogLevel::Info) << "*** Building World ***";

	// Link all the intertwining pointers
	LOG(LogLevel::Info) << "-- Filtering Excess Province Titles";
	provinceTitleMapper.filterSelf(provinces, titles);
	LOG(LogLevel::Info) << "-- Linking Characters With Dynasties";
	characters.linkDynasties(dynasties);
	LOG(LogLevel::Info) << "-- Linking Characters With Lieges and Spouses";
	characters.linkLiegesAndSpouses();
	LOG(LogLevel::Info) << "-- Linking Characters With Primary Titles";
	characters.linkPrimaryTitles(titles);
	LOG(LogLevel::Info) << "-- Linking Characters With Capitals";
	characters.linkCapitals(provinces);
	LOG(LogLevel::Info) << "-- Linking Provinces With Primary Baronies";
	provinces.linkPrimarySettlements();
	LOG(LogLevel::Info) << "-- Linking Titles With Holders";
	titles.linkHolders(characters);
	LOG(LogLevel::Info) << "-- Linking Titles With Liege and DeJure Titles";
	titles.linkLiegePrimaryTitles();
	LOG(LogLevel::Info) << "-- Linking Titles With Vassals and DeJure Vassals";
	titles.linkVassals();
	LOG(LogLevel::Info) << "-- Linking Titles With Provinces";
	titles.linkProvinces(provinces, provinceTitleMapper); // Untestable due to disk access.
	LOG(LogLevel::Info) << "-- Linking Titles With Base Titles";
	titles.linkBaseTitles();

	// Filter top-tier active titles and assign them provinces.
	LOG(LogLevel::Info) << "-- Merging Independent Baronies";
	mergeIndependentBaronies();
	LOG(LogLevel::Info) << "-- Filtering Independent Titles";
	filterIndependentTitles();
	LOG(LogLevel::Info) << "-- Merging Rebellions Into Base";
	mergeRevolts();
	LOG(LogLevel::Info) << "-- Congregating Provinces for Independent Titles";
	congregateProvinces();
	LOG(LogLevel::Info) << "-- Performing Province Sanity Check";
	sanityCheckifyProvinces();


	LOG(LogLevel::Info) << "*** Good-bye CK2, rest in peace. ***";
}

void CK2::World::verifySave(const std::string& saveGamePath)
{
	std::ifstream saveFile(fs::u8path(saveGamePath));
	if (!saveFile.is_open()) throw std::runtime_error("Could not open save! Exiting!");

	char buffer[3];
	saveFile.get(buffer, 3);
	if (buffer[0] == 'P' && buffer[1] == 'K')
	{
		if (!uncompressSave(saveGamePath)) throw std::runtime_error("Failed to unpack the compressed save!");
		saveGame.compressed = true;
	}
	saveFile.close();
}

bool CK2::World::uncompressSave(const std::string& saveGamePath)
{
	auto savefile = ZipFile::Open(saveGamePath);
	if (!savefile) return false;
	for (size_t entryNum = 0; entryNum < savefile->GetEntriesCount(); ++entryNum)
	{
		const auto& entry = savefile->GetEntry(entryNum);
		const auto& name = entry->GetName();
		if (name == "meta")
		{
			LOG(LogLevel::Info) << ">> Uncompressing metadata";
			saveGame.metadata = std::string{std::istreambuf_iterator<char>(*entry->GetDecompressionStream()),
				std::istreambuf_iterator<char>()};
		}
		else if (name == trimPath(saveGamePath))
		{
			LOG(LogLevel::Info) << ">> Uncompressing gamestate";
			saveGame.gamestate = std::string{ std::istreambuf_iterator<char>(*entry->GetDecompressionStream()),
				std::istreambuf_iterator<char>() };
		}
		else throw std::runtime_error("Unrecognized savegame structure!");
	}
	return true;
}

void CK2::World::filterIndependentTitles()
{
	const auto& allTitles = titles.getTitles();
	std::map<std::string, std::shared_ptr<Title>> potentialIndeps;
	
	for (const auto& title: allTitles)
	{
		const auto& liege = title.second->getLiege();
		const auto& holder = title.second->getHolder();
		if (!holder.first) continue; // don't bother with titles without holders.
		if (liege.first.empty())
		{
			// this is a potential indep.
			potentialIndeps.insert(std::pair(title.first, title.second));
			continue;
		}
		const auto& liegeTitle = liege.second->getTitle();
		if (liegeTitle.first == "e_hre")
		{
			// hre emperor's vassals also sound like indeps to me.
			potentialIndeps.insert(std::pair(title.first, title.second));
			continue;
		}
		if (liegeTitle.second->getLiege().first == "e_hre")
		{
			// We're still in hre? Are we a duchy?
			if (title.first.find("d_") == 0)
			{
				// we have a king over us. Well. Not any more.
				potentialIndeps.insert(std::pair(title.first, title.second));
			}
		}
	}

	// Check if we hold any actual land (c_something). (Only necessary for the holder,
	// no need to recurse, we're just filtering landless titular titles like mercenaries
	// or landless Pope. If a character holds a landless titular title along actual title
	// (like Caliphate), it's not relevant at this stage as he's independent anyway.

	// First, split off all county_title holders into a container.
	std::set<int> countyHolders;
	for (const auto& title : allTitles)
	{
		if (title.second->getHolder().first && title.second->getName().find("c_") == 0)
		{
			countyHolders.insert(title.second->getHolder().first);
		}
	}

	// Then look at all potential indeps and see if their holders are up there.
	auto counter = 0;
	for (const auto& indep: potentialIndeps)
	{
		const auto& holderID = indep.second->getHolder().first;
		if (countyHolders.count(holderID))
		{
			// this fellow holds a county, so his indep title is an actual title.
			independentTitles.insert(std::pair(indep.first, indep.second));
			counter++;
		}
	}
	Log(LogLevel::Info) << "<> " << counter << " independent titles recognized.";
}

void CK2::World::mergeIndependentBaronies() const
{
	auto counter = 0;
	const auto& allTitles = titles.getTitles();
	for (const auto& title : allTitles)
	{
		const auto& holder = title.second->getHolder();
		if (!holder.first) continue; // don't bother with titles without holders.
		const auto& liege = title.second->getLiege();
		if (liege.first.empty())
		{
			// this is an indep.
			if (title.first.find("b_") == 0)
			{
				// it's a barony.
				const auto& djLiege = title.second->getDeJureLiege();
				if (djLiege.first.find("c_") == 0)
				{
					// we're golden.
					title.second->overrideLiege();
					counter++;
				}
			}
		}
	}
	Log(LogLevel::Info) << "<> " << counter << " baronies reassigned.";
}

void CK2::World::congregateProvinces()
{
	auto counter = 0;
	// We're linking all contained province for a title's tree under that title.
	// This will form actual EU4 tag and contained provinces.
	for (const auto& title: independentTitles)
	{
		title.second->congregateProvinces(independentTitles);
		counter += title.second->getProvinces().size();
	}
	Log(LogLevel::Info) << "<> " << counter << " provinces held by independents.";
}

void CK2::World::mergeRevolts()
{
	// major revolts need to have their leader drop the top-tier revolt title and relink
	// to revolt's base_title.

	std::set<std::string> droppedRevoltTitles;

	for (const auto& indep: independentTitles)
	{
		if (!indep.second->isMajorRevolt()) continue;
		// for a major revolt, scroll through all vassals and relink them to to base;		
		for (const auto& vassal: indep.second->getVassals())
		{
			const auto& revoltBaseTitle = indep.second->getBaseTitle();
			vassal.second->overrideLiege(revoltBaseTitle);
			const auto& newLiege = vassal.second->getLiege().second->getTitle();
			newLiege.second->registerVassal(std::pair(vassal.first, vassal.second));			
		}
		droppedRevoltTitles.insert(indep.first);
	}
	// finally, clear them out.
	for (const auto& droppedRevolt : droppedRevoltTitles)
	{
		independentTitles.erase(droppedRevolt);
	}
	Log(LogLevel::Info) << "<> " << droppedRevoltTitles.size() << " revolts merged.";
}

void CK2::World::sanityCheckifyProvinces()
{
	// This is a watchdog function intended to complain if multiple independent titles
	// link to a single province.	
	std::map<int, std::vector<std::string>> provinceTitlesMap; // we store all holders for every province.
	
	for (const auto& indep : independentTitles)
	{
		const auto& ownedProvinces = indep.second->getProvinces();
		for (const auto& province: ownedProvinces)
		{
			provinceTitlesMap[province.first].push_back(indep.first);
		}
	}
	// and now, explode.
	for (const auto& entry: provinceTitlesMap)
	{
		if (entry.second.size() > 1)
		{
			std::string warning = "Province ID: " + std::to_string(entry.first) + " is owned by: ";
			for (const auto& owner: entry.second)
			{
				warning += owner + ",";
			}
			Log(LogLevel::Warning) << warning;
		}
	}
}
