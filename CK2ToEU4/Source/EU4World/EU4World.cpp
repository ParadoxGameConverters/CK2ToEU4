#include "EU4World.h"
#include "Log.h"
#include "OSCompatibilityLayer.h"
#include <filesystem>
#include <fstream>
namespace fs = std::filesystem;
#include "../CK2World/Characters/Character.h"
#include "../CK2World/Titles/Title.h"
#include "../Configuration/Configuration.h"
#include <cmath>

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
	distributeHRESubtitles(theConfiguration);
	diplomacy.importAgreements(countries);
	resolvePersonalUnions();

	LOG(LogLevel::Info) << "---> The Dump <---";
	modFile.outname = theConfiguration.getOutputName();
	output(versionParser, theConfiguration, sourceWorld.getConversionDate());
	LOG(LogLevel::Info) << "*** Farewell EU4, granting you independence. ***";
}

void EU4::World::resolvePersonalUnions()
{
	const std::set<std::string> elegibleReligions = {"catholic", "orthodox", "nestorian", "coptic"};
	std::map<int, std::map<std::string, std::shared_ptr<Country>>> holderTitles;
	std::map<int, std::pair<std::string, std::shared_ptr<Country>>> holderPrimaryTitle;
	std::map<int, std::shared_ptr<CK2::Character>> relevantHolders;

	// We're filling the registry first.
	for (const auto& country: countries) {
		if (country.second->getTitle().first.empty()) continue;
		if (!country.second->getTitle().second->getHolder().first) continue;
		// we have a holder.
		const auto& holder = country.second->getTitle().second->getHolder();
		holderTitles[holder.first].insert(country);
		relevantHolders.insert(holder);
		// does he have a primary title?
		if (!holder.second->getPrimaryTitle().first.empty()) {
			if (!holder.second->getPrimaryTitle().second->getTitle().second->getEU4Tag().first.empty()) {
				holderPrimaryTitle[holder.first] = holder.second->getPrimaryTitle().second->getTitle().second->getEU4Tag();
			}
		}
	}

	// Now let's see what we have.
	for (const auto& holderTitle: holderTitles) {
		if (holderTitle.second.size() <= 1) continue;
		// multiple crowns. What's our primary?

		auto primaryItr = holderPrimaryTitle.find(holderTitle.first);
		std::pair<std::string, std::shared_ptr<Country>> primaryTitle;
		if (primaryItr == holderPrimaryTitle.end() || primaryItr->second.second->getProvinces().empty()) {
			// We need to find another primary title.
			auto foundPrimary = false;
			for (const auto& title: holderTitle.second) {
				if (!title.second->getProvinces().empty()) {
					primaryTitle = std::pair(title.first, title.second);
					foundPrimary = true;
					break;
				}
			}
			if (!foundPrimary) continue; // no helping this fellow.
		} else {
			primaryTitle = primaryItr->second;
		}

		// religion
		const auto& religion = primaryTitle.second->getReligion();
		auto heathen = false;
		if (!elegibleReligions.count(religion)) heathen = true;

		// We now have a holder, his primary, and religion. Let's resolve multiple crowns.
		auto unionCount = 0;
		if (!heathen) {
			for (const auto& title: holderTitle.second) {
				if (title.first == primaryTitle.first) continue;
				if (title.second->getProvinces().empty()) continue;
				// Craft a relation. Going up to a max of 3 unions.
				if (unionCount <= 2) {
					diplomacy.addAgreement(Agreement(primaryTitle.first, title.first, "union", primaryTitle.second->getConversionDate()));
					++unionCount;
				} else {
					// too many unions.
					primaryTitle.second->annexCountry(title);
				}
			}
		} else {
			// heathens annex straight up.
			for (const auto& title: holderTitle.second) {
				if (title.first == primaryTitle.first) continue;
				if (title.second->getProvinces().empty()) continue;
				// Craft a relation.
				primaryTitle.second->annexCountry(title);
			}
		}
	}
}


void EU4::World::distributeHRESubtitles(const Configuration& theConfiguration)
{
	if (theConfiguration.getHRE() == ConfigurationDetails::I_AM_HRE::NONE) return;
	LOG(LogLevel::Info) << "-> Locating Emperor";
	// Emperor may or may not be set.
	for (const auto& country: countries)
		if (country.second->isHREEmperor()) {
			emperorTag = country.first;
			break;
		}
	if (!emperorTag.empty()) {
		Log(LogLevel::Info) << "<> Emperor is " << emperorTag;
		setFreeCities();
		setElectors();
	} else
		Log(LogLevel::Info) << "<> Emperor could not be found, no HRE mechanics.";
}

void EU4::World::setElectors()
{
	LOG(LogLevel::Info) << "-> Setting Electors";
	std::vector<std::pair<int, std::shared_ptr<Country>>> bishops;	  // piety-tag
	std::vector<std::pair<int, std::shared_ptr<Country>>> duchies;	  // dev-tag
	std::vector<std::pair<int, std::shared_ptr<Country>>> republics; // dev-tag
	std::vector<std::shared_ptr<Country>> electors;

	// We need to be careful about papacy and orthodox holders
	for (const auto& country: countries) {
		if (country.second->isinHRE()) {
			if (country.second->getProvinces().empty()) continue;
			const auto& holder = country.second->getTitle().second->getHolder();
			if (country.first == "PAP" || holder.second->getPrimaryTitle().first == "k_orthodox") {
				// override to always be elector
				bishops.emplace_back(std::pair(99999, country.second));
				continue;
			}
			// Let's shove all hre members into appropriate categories.
			if (country.second->getGovernment() == "theocracy") {
				bishops.emplace_back(std::pair(lround(holder.second->getPiety()), country.second));
			} else if (country.second->getGovernment() == "monarchy") {
				duchies.emplace_back(std::pair(country.second->getDevelopment(), country.second));
			} else if (country.second->getGovernment() == "republic") {
				republics.emplace_back(std::pair(country.second->getDevelopment(), country.second));
			} // skipping tribal and similar.
		}
	}

	std::sort(bishops.rbegin(), bishops.rend());
	std::sort(duchies.rbegin(), duchies.rend());
	std::sort(republics.rbegin(), republics.rend());

	for (const auto& bishop: bishops) {
		if (electors.size() >= 3) break;
		electors.emplace_back(bishop.second);
	}
	for (const auto& republic: republics) {
		if (electors.size() >= 3) break;
		electors.emplace_back(republic.second);
	}
	for (const auto& duchy: duchies) {
		if (electors.size() >= 7) break;
		electors.emplace_back(duchy.second);
	}

	for (const auto& elector: electors) elector->setElector();
	LOG(LogLevel::Info) << "-> There are " << electors.size() << " electors recognized.";
}

void EU4::World::setFreeCities()
{
	LOG(LogLevel::Info) << "-> Setting Free Cities";
	// How many free cities do we already have?
	auto freeCityNum = 0;
	for (const auto& country: countries) {
		if (country.second->isinHRE() && country.second->getGovernment() == "republic" && country.second->getProvinces().size() == 1 &&
			 country.second->getTitle().second->getGeneratedLiege().first.empty() && freeCityNum < 8) {
			country.second->overrideReforms("free_city");
			++freeCityNum;
		}
	}
	// Can we turn some minors into free cities?
	if (freeCityNum < 8) {
		for (const auto& country: countries) {
			if (country.second->isinHRE() && country.second->getGovernment() != "republic" && !country.second->isHREEmperor() &&
				 country.second->getGovernmentReforms().empty() && country.second->getProvinces().size() == 1 &&
				 country.second->getTitle().second->getGeneratedLiege().first.empty() && freeCityNum < 8) {
				if (country.first == "HAB") continue; // For Iohannes who is sensitive about Austria.
				// GovernmentReforms being empty ensures we're not converting special governments and targeted tags into free cities.
				country.second->overrideReforms("free_city");
				country.second->setGovernment("republic");
				++freeCityNum;
			}
		}
	}
	LOG(LogLevel::Info) << "-> There are " << freeCityNum << " free cities.";
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
		if (!country.second->getReligion().empty() && !country.second->getPrimaryCulture().empty() && !country.second->getTechGroup().empty() &&
			 !country.second->getGFX().empty())
			continue;
		if (country.second->getProvinces().empty()) continue; // No point.

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
		}
		if (country.second->getReligion().empty()) {
			auto max = get_max(religiousCensus);
			country.second->setReligion(max.first);
		}
		if (country.second->getTechGroup().empty()) {
			const auto& techMatch = cultureMapper.getTechGroup(country.second->getPrimaryCulture());
			if (techMatch)
				country.second->setTechGroup(*techMatch);
			else {
				country.second->setTechGroup("western");
				Log(LogLevel::Warning) << country.first << " could not determine technological group, substituting western!";
			}
		}
		if (country.second->getGFX().empty()) {
			const auto& gfxMatch = cultureMapper.getGFX(country.second->getPrimaryCulture());
			if (gfxMatch)
				country.second->setTechGroup(*gfxMatch);
			else {
				country.second->setTechGroup("westerngfx");
				Log(LogLevel::Warning) << country.first << " could not determine GFX, substituting westerngfx!";
			}
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
		if (title.first.find("e_") != 0) continue;
		importCK2Country(title, sourceWorld);
	}
	for (const auto& title: sourceWorld.getIndepTitles()) {
		if (title.first.find("k_") != 0) continue;
		importCK2Country(title, sourceWorld);
	}
	for (const auto& title: sourceWorld.getIndepTitles()) {
		if (title.first.find("d_") != 0) continue;
		importCK2Country(title, sourceWorld);
	}
	for (const auto& title: sourceWorld.getIndepTitles()) {
		if (title.first.find("c_") != 0) continue;
		importCK2Country(title, sourceWorld);
	}
	LOG(LogLevel::Info) << ">> " << countries.size() << " total countries recognized.";
}

void EU4::World::importCK2Country(const std::pair<std::string, std::shared_ptr<CK2::Title>>& title, const CK2::World& sourceWorld)
{
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


void EU4::World::importCK2Provinces(const CK2::World& sourceWorld)
{
	LOG(LogLevel::Info) << "-> Importing CK2 Provinces";
	auto counter = 0;
	// CK2 provinces map to a subset of eu4 provinces. We'll only rewrite those we are responsible for.
	for (const auto& province: provinces) {
		const auto& ck2Provinces = provinceMapper.getCK2ProvinceNumbers(province.first);
		// Provinces we're not affecting will not be in this list.
		if (ck2Provinces.empty()) continue;
		// Next, we find what province to use as its initializing source.
		const auto& sourceProvince = determineProvinceSource(ck2Provinces, sourceWorld);
		if (!sourceProvince) continue; // bailing for mismaps.
		// And finally, initialize it.
		province.second->initializeFromCK2(sourceProvince->second, cultureMapper, religionMapper);
		counter++;
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

std::optional<std::pair<int, std::shared_ptr<CK2::Province>>> EU4::World::determineProvinceSource(const std::vector<int>& ck2ProvinceNumbers,
	 const CK2::World& sourceWorld) const
{
	// determine ownership by province development.
	std::map<std::string, std::vector<std::shared_ptr<CK2::Province>>> theClaims; // title, offered province sources
	std::map<std::string, int> theShares;														// title, development
	std::string winner;
	auto maxDev = 0;

	for (auto ck2ProvinceID: ck2ProvinceNumbers) {
		const auto& ck2province = sourceWorld.getProvinces().find(ck2ProvinceID);
		if (ck2province == sourceWorld.getProvinces().end()) continue; // Broken mapping?
		auto ownerTitle = ck2province->second->getTitle().first;
		if (ownerTitle.empty()) continue; // Final sanity check.
		theClaims[ownerTitle].push_back(ck2province->second);
		theShares[ownerTitle] = lround(ck2province->second->getBuildingWeight());

		// While at it, is this province especially important? Enough so we'd sidestep regular rules?
		if (ck2province->second->getTitle().second->isHREEmperor()) {
			const auto& emperor = ck2province->second->getTitle().second->getHolder().second;
			if (emperor->getCapitalProvince().first == ck2province->first) {
				// This is the empire capital, don't assign it away.
				winner = ck2province->second->getTitle().first;
				maxDev = 999;
			}
		}
	}
	// Let's see who the lucky winner is.
	for (const auto& share: theShares) {
		if (share.second > maxDev) {
			winner = share.first;
			maxDev = share.second;
		}
	}
	if (winner.empty()) return std::nullopt;

	// Now that we have a winning title, let's find its largest province to use as a source.
	maxDev = 0;
	std::pair<int, std::shared_ptr<CK2::Province>> toReturn;
	for (const auto& province: theClaims[winner]) {
		if (province->getBuildingWeight() > maxDev) {
			toReturn.first = province->getID();
			toReturn.second = province;
		}
	}
	return toReturn;
}
