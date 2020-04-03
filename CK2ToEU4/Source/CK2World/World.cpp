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
#include "Characters/Character.h"

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
	filterExcessProvinceTitles();
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
	LOG(LogLevel::Info) << "-- Linking Titles With Liege and DeJure titles";
	titles.linkLiegePrimaryTitles();
	 
	// Filter top-tier active titles and assign them provinces.
	LOG(LogLevel::Info) << "-- Merging Independent Baronies";
	mergeIndependentBaronies();
	LOG(LogLevel::Info) << "-- Filtering Independent Titles";
	filterIndependentTitles();


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

void CK2::World::filterExcessProvinceTitles()
{
	// This function's purpose is to filter out invalid provinceID-title mappings from /history/provinces.
	
	const auto& provinceTitles = provinceTitleMapper.getProvinceTitles(); // contains junk.
	std::map<std::string, int> newProvinceTitles;
	const auto& availableTitles = titles.getTitles();

	for (const auto& provinceTitle: provinceTitles)
	{
		if (availableTitles.count(provinceTitle.first)) newProvinceTitles.insert(std::pair(provinceTitle.first, provinceTitle.second));
	}
	Log(LogLevel::Info) << "<> Dropped " << provinceTitles.size() - newProvinceTitles.size() << " invalid mappings.";
	provinceTitleMapper.replaceProvinceTitles(newProvinceTitles);
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
	// Check if we hold any actual land.
	for (const auto& indep: potentialIndeps)
	{
		Log(LogLevel::Debug) << "title: " << indep.second->getName() << " holder: " << indep.second->getHolder().second->getName();
	}
}

void CK2::World::mergeIndependentBaronies()
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
