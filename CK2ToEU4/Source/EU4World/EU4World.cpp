#include "EU4World.h"
#include "../CK2World/Characters/Character.h"
#include "../CK2World/Titles/Title.h"
#include "../CK2World/World.h"
#include "../Configuration/Configuration.h"
#include "../Mappers/VersionParser/VersionParser.h"
#include "Country/Country.h"
#include "Log.h"
#include "OSCompatibilityLayer.h"
#include <filesystem>
#include <fstream>
namespace fs = std::filesystem;

EU4::World::World(const CK2::World& sourceWorld,
	 const Configuration& theConfiguration,
	 const mappers::VersionParser& versionParser)
{
	LOG(LogLevel::Info) << "*** Hello EU4, let's get painting. ***";
	provinceMapper.determineValidProvinces(theConfiguration);
	importVanillaCountries(theConfiguration.getEU4Path());
	importCK2Countries(sourceWorld);
	LOG(LogLevel::Info) << "*** Farewell EU4, granting you independence. ***";
}

void EU4::World::importCK2Countries(const CK2::World& sourceWorld)
{
	LOG(LogLevel::Info) << "-> Importing CK2 Countries";
	
	// countries holds all tags imported from EU4. We'll now overwrite some and
	// add new ones from ck2 titles.
	for (const auto& title: sourceWorld.getIndepTitles()) {

		// Grabbing the capital, if possible
		int eu4CapitalID = 0;
		const auto& ck2CapitalID = title.second->getHolder().second->getCapitalProvince().first;
		if (ck2CapitalID) {
			const auto& capitalMatch = provinceMapper.getEU4ProvinceNumbers(ck2CapitalID);
			if (!capitalMatch.empty()) eu4CapitalID = *capitalMatch.begin();
		}
		Log(LogLevel::Debug) << "cap: " << eu4CapitalID;

		// Mapping the title to a tag
		const auto& tag = titleTagMapper.getTagForTitle(title.first, title.second->getBaseTitle().first, eu4CapitalID);
		if (!tag) throw std::runtime_error("Title " + title.first + " could not be mapped!");
		Log(LogLevel::Debug) << "Mapped to tag: " << *tag;
		
		// Locating appropriate existing country
		const auto& countryItr = countries.find(*tag);
		if (countryItr != countries.end()) {
			Log(LogLevel::Debug) << "Located country, initing: " << *tag;
			countryItr->second->initializeFromTitle(*tag, title.second);
			Log(LogLevel::Debug) << "Init complete: " << *tag;
		} else {
			// Otherwise create the country
			Log(LogLevel::Debug) << "Creating new country: " << *tag;
			std::shared_ptr<Country> newCountry;
			Log(LogLevel::Debug) << "Initing new country: " << *tag;
			newCountry->initializeFromTitle(*tag, title.second);
			Log(LogLevel::Debug) << "Inserting new country: " << *tag;
			countries.insert(std::pair(*tag, newCountry));
			Log(LogLevel::Debug) << "Done: " << *tag;
		}
	}
	LOG(LogLevel::Info) << ">> " << countries.size() << " total countries recognized.";
}


void EU4::World::importVanillaCountries(const std::string& eu4Path)
{
	LOG(LogLevel::Info) << "-> Importing Vanilla Countries";
	// ---- Loading common/countries/
	std::ifstream eu4CountriesFile(fs::u8path(eu4Path + "/common/country_tags/00_countries.txt"));
	if (!eu4CountriesFile.is_open())
		throw std::runtime_error("Could not open " + eu4Path + "/common/country_tags/00_countries.txt!");

	while (!eu4CountriesFile.eof()) {
		std::string line;
		getline(eu4CountriesFile, line);

		if (line[0] == '#' || line.length() < 4) continue;
		auto tag = line.substr(0, 3);

		// All file paths are in quotes. The ones outside are commented, so we can use those as markers.
		auto quoteLoc = line.find_first_of('\"');
		auto countryLine = line.substr(quoteLoc + 1, line.length());
		quoteLoc = countryLine.find_last_of('\"');
		countryLine = countryLine.substr(0, quoteLoc);
		const auto filePath = eu4Path + "/common/" + countryLine;

		// We're soaking up all vanilla countries with all current definitions.
		const auto newCountry = std::make_shared<Country>(tag, filePath);
		countries.insert(std::make_pair(tag, newCountry));
	}
	eu4CountriesFile.close();
	LOG(LogLevel::Info) << ">> Loaded " << countries.size() << " countries.";

	LOG(LogLevel::Info) << "-> Importing Vanilla Country History";
	// ---- Loading history/countries/
	std::set<std::string> fileNames;
	Utils::GetAllFilesInFolder(eu4Path + "/history/countries/", fileNames);
	for (const auto& fileName: fileNames) {
		auto tag = fileName.substr(0, 3);
		countries[tag]->loadHistory(eu4Path + "/history/countries/" + fileName);
	}
	LOG(LogLevel::Info) << ">> Loaded " << fileNames.size() << " history files.";
}


/*
void EU4::World::loadRegions(const Configuration& theConfiguration)
{
	// @idhrendur: This section will be relevant for later
	//
	//LOG(LogLevel::Info) << "-> Loading Geographical Structure";
	loadRegions(theConfiguration);
	auto areaFilename = theConfiguration.getEU4Path() + "/map/area.txt";
	auto regionFilename = theConfiguration.getEU4Path() + "/map/region.txt";
	auto superRegionFilename = theConfiguration.getEU4Path() + "/map/superregion.txt";

	std::ifstream areaStream(fs::u8path(areaFilename));
	if (!areaStream.is_open()) throw std::runtime_error("Could not open map/area.txt!");
	//Areas installedAreas(areaStream);
	areaStream.close();
	//assignProvincesToAreas(installedAreas.getAreas());

	std::ifstream superRegionFile(fs::u8path(superRegionFilename));
	if (!superRegionFile.is_open()) throw std::runtime_error("Could not open map/superregion.txt!");
	//SuperRegions sRegions(superRegionFile);
	superRegionFile.close();

	std::ifstream regionStream(fs::u8path(regionFilename));
	if (!regionStream.is_open()) throw std::runtime_error("Could not open map/region.txt!");
	//regions = std::make_unique<Regions>(sRegions, installedAreas, regionStream);
	regionStream.close();
}
*/