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
	localizationMapper.scrapeLocalizations(theConfiguration);
	colorScraper.scrapeColors(theConfiguration.getCK2Path() + "/common/landed_titles/landed_titles.txt");
	regionMapper = std::make_shared<mappers::RegionMapper>();
	regionMapper->loadRegions(theConfiguration);
	cultureMapper.loadRegionMapper(regionMapper);
	provinceMapper.determineValidProvinces(theConfiguration);
	importVanillaCountries(theConfiguration.getEU4Path());
	importCK2Countries(sourceWorld);
	importVanillaProvinces(theConfiguration.getEU4Path());
	regionMapper->linkProvinces(provinces);
	importCK2Provinces(sourceWorld);
	linkProvincesToCountries();
	verifyReligionsAndCultures();

	LOG(LogLevel::Info) << "---> The Dump <---";
	modFile.outname = theConfiguration.getOutputName();
	output(versionParser, theConfiguration, sourceWorld.getConversionDate());
	LOG(LogLevel::Info) << "*** Farewell EU4, granting you independence. ***";
}

void EU4::World::linkProvincesToCountries()
{
	// Some of the provinces have linked countries, but new world won't. We need to insert links both ways there.
	for (const auto& province: provinces) {
		if (province.second->getOwner().empty()) continue; // this is the uncolonized case
		const auto& countryItr = countries.find(province.second->getOwner());
		if (countryItr != countries.end()) {
			// registering owner in province
			if (province.second->getTagCountry().first.empty()) province.second->registerTagCountry(std::pair(countryItr->first, countryItr->second));
			// registering province in owner.
			countryItr->second->registerProvince(std::pair(province.first, province.second));
		} else {
			Log(LogLevel::Warning) << "Province " << province.first << " owner " << province.second->getOwner() << " has no country!";
		}
	}
}

template <typename KeyType, typename ValueType> std::pair<KeyType, ValueType> get_max(const std::map<KeyType, ValueType>& x)
{
	using pairtype = std::pair<KeyType, ValueType>;
	return *std::max_element(x.begin(), x.end(), [](const pairtype& p1, const pairtype& p2) { return p1.second < p2.second; });
}

void EU4::World::verifyReligionsAndCultures()
{
	// We are checking every country if it lacks primary religion and culture. This is an issue for hordeland mainly.
	// For those lacking setups, we'll do a provincial census and inherit those values.
	for (const auto& country: countries) {
		if (!country.second->getReligion().empty() && !country.second->getPrimaryCulture().empty()) continue;
		std::map<std::string, int> religiousCensus;
		std::map<std::string, int> culturalCensus;
		for (const auto& province: country.second->getProvinces()) {
			if (province.second->getReligion().empty()) {
				Log(LogLevel::Warning) << "Province " << province.first << " has no religion set!";
				continue;
			}
			if (province.second->getCulture().empty()) {
				Log(LogLevel::Warning) << "Province " << province.first << " has no culture set!";
				continue;
			}
			religiousCensus[province.second->getReligion()] += 1;
			culturalCensus[province.second->getCulture()] += 1;
		}
		if (country.second->getPrimaryCulture().empty()) {
			auto max = get_max(culturalCensus);
			country.second->setPrimaryCulture(max.first);
			Log(LogLevel::Debug) << "Setting " << country.first << " culture to " << max.first;
		}
		if (country.second->getReligion().empty()) {
			auto max = get_max(religiousCensus);
			country.second->setReligion(max.first);
			Log(LogLevel::Debug) << "Setting " << country.first << " religion to " << max.first;
		}
	}
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
			countryItr->second->initializeFromTitle(*tag,
				 title.second,
				 governmentsMapper,
				 religionMapper,
				 cultureMapper,
				 provinceMapper,
				 colorScraper,
				 localizationMapper,
				 sourceWorld.getConversionDate());
			title.second->registerEU4Tag(std::pair(*tag, countryItr->second));
		} else {
			// Otherwise create the country
			auto newCountry = std::make_shared<Country>();
			newCountry->initializeFromTitle(*tag,
				 title.second,
				 governmentsMapper,
				 religionMapper,
				 cultureMapper,
				 provinceMapper,
				 colorScraper,
				 localizationMapper,
				 sourceWorld.getConversionDate());
			title.second->registerEU4Tag(std::pair(*tag, newCountry));
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
				provinceItr->second->initializeFromCK2(province.second, cultureMapper, religionMapper);
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
