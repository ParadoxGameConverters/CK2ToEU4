#ifndef EU4_WORLD_H
#define EU4_WORLD_H
#include "../CK2World/World.h"
#include "../Mappers/ColorScraper/ColorScraper.h"
#include "../Mappers/CultureMapper/CultureMapper.h"
#include "../Mappers/GovernmentsMapper/GovernmentsMapper.h"
#include "../Mappers/LocalizationMapper/LocalizationMapper.h"
#include "../Mappers/ProvinceMapper/ProvinceMapper.h"
#include "../Mappers/RegionMapper/RegionMapper.h"
#include "../Mappers/ReligionMapper/ReligionMapper.h"
#include "../Mappers/TitleTagMapper/TitleTagMapper.h"
#include "../Mappers/VersionParser/VersionParser.h"
#include "../Mappers/RulerPersonalitiesMapper/RulerPersonalitiesMapper.h"
#include "Country/Country.h"
#include "Diplomacy/Diplomacy.h"
#include "Output/outModFile.h"
#include "Province/EU4Province.h"

class Configuration;

namespace CK2
{
class Province;
} // namespace CK2

namespace EU4
{
class World
{
  public:
	World(const CK2::World& sourceWorld, const Configuration& theConfiguration, const mappers::VersionParser& versionParser);

  private:
	// void loadRegions(const Configuration& theConfiguration); waiting on geography.
	void importVanillaCountries(const std::string& eu4Path, bool invasion);
	void loadCountriesFromSource(std::istream& theStream, const std::string& sourcePath, bool isVanillaSource);
	void importVanillaProvinces(const std::string& eu4Path, bool invasion);
	void importCK2Countries(const CK2::World& sourceWorld);
	void importCK2Country(const std::pair<std::string, std::shared_ptr<CK2::Title>>& title, const CK2::World& sourceWorld);
	void importCK2Provinces(const CK2::World& sourceWorld);
	void output(const mappers::VersionParser& versionParser, const Configuration& theConfiguration, date conversionDate, bool invasion) const;
	void createModFile(const Configuration& theConfiguration) const;
	void outputVersion(const mappers::VersionParser& versionParser, const Configuration& theConfiguration) const;
	void outputCommonCountriesFile(const Configuration& theConfiguration) const;
	void outputHistoryCountries(const Configuration& theConfiguration) const;
	void outputHistoryProvinces(const Configuration& theConfiguration) const;
	void outputCommonCountries(const Configuration& theConfiguration) const;
	void outputLocalization(const Configuration& theConfiguration, bool invasion) const;
	void verifyReligionsAndCultures();
	void linkProvincesToCountries();
	void outputFlags(const Configuration& theConfiguration, bool invasion) const;
	void outputBookmark(const Configuration& theConfiguration, date conversionDate) const;
	void distributeHRESubtitles(const Configuration& theConfiguration);
	void outputEmperor(const Configuration& theConfiguration, date conversionDate) const;
	[[nodiscard]] std::optional<std::pair<int, std::shared_ptr<CK2::Province>>> determineProvinceSource(const std::vector<int>& ck2ProvinceNumbers,
		 const CK2::World& sourceWorld) const;
	void setElectors();
	void setFreeCities();
	void outputDiplomacy(const Configuration& theConfiguration, const std::vector<Agreement>& agreements, bool invasion) const;
	void resolvePersonalUnions();
	void importAdvisers();
	void outputAdvisers(const Configuration& theConfiguration) const;
	void alterProvinceDevelopment();

	mappers::ColorScraper colorScraper;
	mappers::ProvinceMapper provinceMapper;
	mappers::TitleTagMapper titleTagMapper;
	mappers::ReligionMapper religionMapper;
	std::shared_ptr<mappers::RegionMapper> regionMapper;
	ModFile modFile;
	std::map<std::string, std::shared_ptr<Country>> countries;
	std::map<int, std::shared_ptr<Province>> provinces;
	mappers::CultureMapper cultureMapper;
	mappers::GovernmentsMapper governmentsMapper;
	mappers::LocalizationMapper localizationMapper;
	mappers::RulerPersonalitiesMapper rulerPersonalitiesMapper;
	std::string emperorTag;
	Diplomacy diplomacy;

	std::set<std::string> specialCountryTags; // tags we loaded from own sources and must not output into 00_country_tags.txt
};
} // namespace EU4

#endif // EU4_WORLD_H