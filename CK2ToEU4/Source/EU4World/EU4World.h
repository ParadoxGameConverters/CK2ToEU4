#ifndef EU4_WORLD_H
#define EU4_WORLD_H
#include "../CK2World/World.h"
#include "../Mappers/AfricanPassesMapper/AfricanPassesMapper.h"
#include "../Mappers/ColorScraper/ColorScraper.h"
#include "../Mappers/CultureMapper/CultureMapper.h"
#include "../Mappers/DevWeightsMapper/DevWeightsMapper.h"
#include "../Mappers/GovernmentsMapper/GovernmentsMapper.h"
#include "../Mappers/LocalizationMapper/LocalizationMapper.h"
#include "../Mappers/PrimaryTagMapper/PrimaryTagMapper.h"
#include "../Mappers/ProvinceMapper/ProvinceMapper.h"
#include "../Mappers/RegionMapper/RegionMapper.h"
#include "../Mappers/ReligionMapper/ReligionMapper.h"
#include "../Mappers/RulerPersonalitiesMapper/RulerPersonalitiesMapper.h"
#include "../Mappers/TitleTagMapper/TitleTagMapper.h"
#include "ConverterVersion.h"
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
	World(const CK2::World& sourceWorld, const Configuration& theConfiguration, const commonItems::ConverterVersion& converterVersion);

  private:
	// void loadRegions(const Configuration& theConfiguration); waiting on geography.
	void importVanillaCountries(const std::string& eu4Path, bool invasion);
	void loadCountriesFromSource(std::istream& theStream, const std::string& sourcePath, bool isVanillaSource);
	void importVanillaProvinces(const std::string& eu4Path, bool invasion);
	void importCK2Countries(Configuration::STARTDATE startDateOption, const CK2::World& sourceWorld);
	void importCK2Country(const std::pair<std::string, std::shared_ptr<CK2::Title>>& title,
		 Configuration::STARTDATE startDateOption,
		 const CK2::World& sourceWorld);
	void importCK2Provinces(const CK2::World& sourceWorld);
	void output(const commonItems::ConverterVersion& converterVersion, const Configuration& theConfiguration, const CK2::World& sourceWorld) const;
	void createModFile(const Configuration& theConfiguration) const;
	void outputVersion(const commonItems::ConverterVersion& converterVersion, const Configuration& theConfiguration) const;
	void outputCommonCountriesFile(const Configuration& theConfiguration) const;
	void outputHistoryCountries(const Configuration& theConfiguration) const;
	void outputHistoryProvinces(const Configuration& theConfiguration, const std::set<std::string>& premades, const bool& isLeviathanDLCPresent) const;
	void outputInvasionExtras(const Configuration& theConfiguration, bool invasion) const;
	void outputCommonCountries(const Configuration& theConfiguration) const;
	void outputLocalization(const Configuration& theConfiguration, bool invasion, bool greekReformation) const;
	void verifyReligionsAndCultures();
	void linkProvincesToCountries();
	void outputFlags(const Configuration& theConfiguration, const CK2::World& sourceWorld) const;
	void outputBookmark(const Configuration& theConfiguration, date conversionDate) const;
	void distributeHRESubtitles(const Configuration& theConfiguration);
	void outputEmperor(const Configuration& theConfiguration, date conversionDate) const;
	void outputDynamicInstitutions(const Configuration& theConfiguration) const;
	void setElectors();
	void setFreeCities();
	void outputDiplomacy(const Configuration& theConfiguration, const std::vector<std::shared_ptr<Agreement>>& agreements, bool invasion) const;
	void outputReformedReligions(const Configuration& theConfiguration,
		 bool noReformation,
		 const std::vector<mappers::ReformedReligionMapping>&,
		 const std::vector<mappers::ReformedReligionMapping>&) const;
	void resolvePersonalUnions();
	void importAdvisers(Configuration::STARTDATE startDateOption, date theConversionDate);
	void outputAdvisers(const Configuration& theConfiguration) const;
	void alterProvinceDevelopment();
	void distributeForts();
	void verifyCapitals();
	void adjustChina(const CK2::World& sourceWorld, const Configuration::STARTDATE& startDateOption);
	void fixTengri();
	void siberianQuestion(const Configuration& theConfiguration);
	void distributeClaims(const Configuration& theConfiguration);
	void distributeDeadCores();
	void scrapeColors(const Configuration& theConfiguration, const CK2::World& sourceWorld);
	void assignAllCountryReforms();
	void africaQuestion();
	void indianQuestion();
	void fixDuplicateNames();
	void markHRETag(const Configuration& theConfiguration, const std::string& hreTitle);

	[[nodiscard]] std::optional<std::pair<int, std::shared_ptr<CK2::Province>>> determineProvinceSource(const std::vector<int>& ck2ProvinceNumbers,
		 const CK2::World& sourceWorld) const;

	bool tianxia = false;
	bool cleanslate = false;

	std::string emperorTag;
	std::string celestialEmperorTag;
	std::string actualHRETag;
	std::map<std::string, std::shared_ptr<Country>> countries;
	std::map<int, std::shared_ptr<Province>> provinces;
	std::shared_ptr<mappers::RegionMapper> regionMapper;
	std::set<std::string> specialCountryTags; // tags we loaded from own sources and must not output into 00_country_tags.txt

	mappers::ColorScraper colorScraper;
	std::unique_ptr<mappers::ProvinceMapper> provinceMapper;
	mappers::TitleTagMapper titleTagMapper;
	mappers::ReligionMapper religionMapper;
	mappers::CultureMapper cultureMapper;
	mappers::GovernmentsMapper governmentsMapper;
	mappers::LocalizationMapper localizationMapper;
	mappers::RulerPersonalitiesMapper rulerPersonalitiesMapper;
	mappers::PrimaryTagMapper primaryTagMapper;
	mappers::DevWeightsMapper devWeightsMapper;
	mappers::AfricanPassesMapper africanPassesMapper;
	ModFile modFile;
	Diplomacy diplomacy;
};
} // namespace EU4

#endif // EU4_WORLD_H
