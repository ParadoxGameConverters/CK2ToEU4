#include "EU4World.h"
#include "../CK2World/World.h"
#include "../Configuration/Configuration.h"
#include "../Mappers/VersionParser/VersionParser.h"
#include "Country/Country.h"
#include "Log.h"
#include <filesystem>
#include <fstream>
#include "OSCompatibilityLayer.h"
namespace fs = std::filesystem;

EU4::World::World(const CK2::World& sourceWorld,
	 const Configuration& theConfiguration,
	 const mappers::VersionParser& versionParser)
{
	LOG(LogLevel::Info) << "*** Hello EU4, let's get painting. ***";
	provinceMapper.determineValidProvinces(theConfiguration);
	importVanillaCountries(theConfiguration.getEU4Path());
	LOG(LogLevel::Info) << "*** Farewell EU4, granting you independence. ***";
}

void EU4::World::importVanillaCountries(const std::string& eu4Path)
{
	LOG(LogLevel::Info) << "-> Importing Vanilla Countries";
	// ---- Loading common/countries/
	std::ifstream eu4CountriesFile(fs::u8path(eu4Path + "/common/country_tags/00_countries.txt"));
	if (!eu4CountriesFile.is_open())
		throw std::runtime_error("Could not open " + eu4Path + "/common/country_tags/00_countries.txt!");

	while (!eu4CountriesFile.eof())
	{
		std::string line;
		getline(eu4CountriesFile, line);

		if (line[0] == '#' || line.length() < 4)
			continue;
		auto tag = line.substr(0, 3);

		// All file paths are in quotes. The ones outside are commented, so we can use those as markers.
		auto quoteLoc = line.find_first_of('\"');
		auto countryLine = line.substr(quoteLoc + 1, line.length());
		quoteLoc = countryLine.find_last_of('\"');
		countryLine = countryLine.substr(0, quoteLoc);
		const auto filePath = eu4Path + "/common/" + countryLine;

		// We're soaking up all vanilla countries with all current definitions.
		const auto newCountry = std::make_shared<Country>(tag, filePath);
		vanillaCountries.insert(std::make_pair(tag, newCountry));
	}
	eu4CountriesFile.close();
	LOG(LogLevel::Info) << ">> Loaded " << vanillaCountries.size() << " countries.";

	LOG(LogLevel::Info) << "-> Importing Vanilla Country History";
	// ---- Loading history/countries/
	std::set<std::string> fileNames;
	Utils::GetAllFilesInFolder(eu4Path + "/history/countries/", fileNames);
	for (const auto& fileName: fileNames)
	{
		auto tag = fileName.substr(0, 3);
		vanillaCountries[tag]->loadHistory(eu4Path + "/history/countries/" + fileName);
	}
	LOG(LogLevel::Info) << ">> Loaded " << fileNames.size() << " history files.";
}


/*
void EU4::World::loadRegions(const Configuration& theConfiguration)
{
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