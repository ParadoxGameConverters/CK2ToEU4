#include "EU4World.h"
#include "Log.h"
#include "OSCompatibilityLayer.h"
#include <filesystem>
#include <fstream>
namespace fs = std::filesystem;
#include "../CK2World/Characters/Character.h"
#include "../CK2World/Titles/Title.h"
#include "../Configuration/Configuration.h"

EU4::World::World(const CK2::World& sourceWorld, const Configuration& theConfiguration, const mappers::VersionParser& versionParser)
{
	LOG(LogLevel::Info) << "*** Hello EU4, let's get painting. ***";
	regionMapper = std::make_shared<mappers::RegionMapper>();
	regionMapper->loadRegions(theConfiguration);
	cultureMapper.loadRegionMapper(regionMapper);
	provinceMapper.determineValidProvinces(theConfiguration);
	importVanillaCountries(theConfiguration.getEU4Path());
	importCK2Countries(sourceWorld);
	importVanillaProvinces(theConfiguration.getEU4Path());
	regionMapper->linkProvinces(provinces);
	importCK2Provinces(sourceWorld);
	LOG(LogLevel::Info) << "---> The Dump <---";
	output(versionParser, theConfiguration);
	LOG(LogLevel::Info) << "*** Farewell EU4, granting you independence. ***";
}

void EU4::World::importVanillaProvinces(const std::string& eu4Path)
{
	LOG(LogLevel::Info) << "-> Importing Vanilla Provinces";
	// ---- Loading history/provinces
	std::set<std::string> fileNames;
	Utils::GetAllFilesInFolder(eu4Path + "/history/provinces/", fileNames);
	for (const auto& fileName: fileNames) {
		const auto minusLoc = fileName.find(" - ");
		const auto id = std::stoi(fileName.substr(0, minusLoc));
		auto newProvince = std::make_shared<Province>(id, eu4Path + "/history/provinces/" + fileName);
		provinces.insert(std::pair(id, newProvince));
	}
	LOG(LogLevel::Info) << ">> Loaded " << provinces.size() << " province definitions.";
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

		// Mapping the title to a tag
		const auto& tag = titleTagMapper.getTagForTitle(title.first, title.second->getBaseTitle().first, eu4CapitalID);
		if (!tag) throw std::runtime_error("Title " + title.first + " could not be mapped!");

		// Locating appropriate existing country
		const auto& countryItr = countries.find(*tag);
		if (countryItr != countries.end()) {
			countryItr->second->initializeFromTitle(*tag, title.second);
		} else {
			// Otherwise create the country
			auto newCountry = std::make_shared<Country>();
			newCountry->initializeFromTitle(*tag, title.second);
			countries.insert(std::pair(*tag, newCountry));
		}
	}
	LOG(LogLevel::Info) << ">> " << countries.size() << " total countries recognized.";
}


void EU4::World::importCK2Provinces(const CK2::World& sourceWorld)
{
	LOG(LogLevel::Info) << "-> Importing CK2 Provinces";
	auto counter = 0;
	// CK2 provinces map to a subset of eu4 provinces. We'll only rewrite those we are responsible for.
	for (const auto& province: sourceWorld.getProvinces()) {

		// every ck2 province can map to a number of eu4 ones.
		const auto& eu4Provinces = provinceMapper.getEU4ProvinceNumbers(province.first);
		for (const auto& eu4Province: eu4Provinces) {
			// Locating appropriate existing province, and this should never fail
			const auto& provinceItr = provinces.find(eu4Province);
			if (provinceItr != provinces.end()) {
				provinceItr->second->initializeFromCK2(province.second);
				counter++;
			} else {
				// Otherwise make a fuss!
				Log(LogLevel::Warning) << "CK2 province " << province.first << " mapped to EU4 " << eu4Province << " has no definition loaded!";
			}
		}
	}
	LOG(LogLevel::Info) << ">> " << sourceWorld.getProvinces().size() << " CK2 provinces imported into " << counter << " EU4 provinces.";
}

void EU4::World::importVanillaCountries(const std::string& eu4Path)
{
	LOG(LogLevel::Info) << "-> Importing Vanilla Countries";
	// ---- Loading common/countries/
	std::ifstream eu4CountriesFile(fs::u8path(eu4Path + "/common/country_tags/00_countries.txt"));
	if (!eu4CountriesFile.is_open()) throw std::runtime_error("Could not open " + eu4Path + "/common/country_tags/00_countries.txt!");

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
