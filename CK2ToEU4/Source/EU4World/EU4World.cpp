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

EU4::World::World(const CK2::World& sourceWorld, const Configuration& theConfiguration, const mappers::VersionParser& versionParser)
{
	LOG(LogLevel::Info) << "*** Hello EU4, let's get painting. ***";
	provinceMapper.determineValidProvinces(theConfiguration);
	importVanillaCountries(theConfiguration.getEU4Path());
	importCK2Countries(sourceWorld);
	LOG(LogLevel::Info) << "---> The Dump <---";
	output(versionParser, theConfiguration);
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

void EU4::World::output(const mappers::VersionParser& versionParser, const Configuration& theConfiguration) const
{
	LOG(LogLevel::Info) << "<- Creating Output Folder";
	fs::create_directory("output");
	if (fs::exists("output/" + theConfiguration.getOutputName())) {
		Log(LogLevel::Info) << "<< Deleting existing mod filder.";
		fs::remove_all("output/" + theConfiguration.getOutputName());
	}

	fs::create_directory("output/" + theConfiguration.getOutputName());
	fs::create_directory("output/" + theConfiguration.getOutputName() + "/history/");
	fs::create_directory("output/" + theConfiguration.getOutputName() + "/history/countries/");
	fs::create_directory("output/" + theConfiguration.getOutputName() + "/common/");
	fs::create_directory("output/" + theConfiguration.getOutputName() + "/common/countries/");

	LOG(LogLevel::Info) << "<- Crafting .mod File";
	createModFile(theConfiguration);

	// Record converter version
	LOG(LogLevel::Info) << "<- Writing version";
	outputVersion(versionParser, theConfiguration);

	// Output common\countries.txt
	LOG(LogLevel::Info) << "<- Creating countries.txt";
	outputCommonCountries(theConfiguration);

	LOG(LogLevel::Info) << "<- Writing Countries";
	outputCommonCountries(theConfiguration);
	outputHistoryCountries(theConfiguration);
}

void EU4::World::createModFile(const Configuration& theConfiguration) const
{
	std::ofstream output("output/" + theConfiguration.getOutputName() + ".mod");
	if (!output.is_open()) throw std::runtime_error("Could not create " + theConfiguration.getOutputName() + ".mod");
	LOG(LogLevel::Info) << "<< Writing to: "
							  << "output/" + theConfiguration.getOutputName() + ".mod";
	output << modFile;
	output.close();
}


void EU4::World::outputVersion(const mappers::VersionParser& versionParser, const Configuration& theConfiguration) const
{
	std::ofstream output("output/" + theConfiguration.getOutputName() + "/ck2toeu4_version.txt");
	if (!output.is_open()) throw std::runtime_error("Error writing version file! Is the output folder writable?");
	output << versionParser;
	output.close();
}

void EU4::World::outputCommonCountriesFile(const Configuration& theConfiguration) const
{
	std::ofstream output("output/" + theConfiguration.getOutputName() + "/common/countries.txt");
	if (!output.is_open()) throw std::runtime_error("Could not create countries file!");

	for (const auto& country: countries) { output << country.first << " = \"" << country.second->getCommonCountryFile() << "\"\n"; }
	output << "\n";
	output.close();
}

void EU4::World::outputHistoryCountries(const Configuration& theConfiguration) const
{
	for (const auto& country: countries) {
		std::ofstream output("output/" + theConfiguration.getOutputName() + "/" + country.second->getHistoryCountryFile());
		if (!output.is_open())
			throw std::runtime_error("Could not create country history file: output/" + theConfiguration.getOutputName() + "/" + country.second->getHistoryCountryFile());
		output << *country.second;
		output.close();
	}
}

void EU4::World::outputCommonCountries(const Configuration& theConfiguration) const
{
	for (const auto& country: countries) {
		std::ofstream output("output/" + theConfiguration.getOutputName() + "/" + country.second->getCommonCountryFile());
		if (!output.is_open())
			throw std::runtime_error("Could not create country common file: output/" + theConfiguration.getOutputName() + "/" + country.second->getCommonCountryFile());
		country.second->outputCommons(output);
		output.close();
	}
}
