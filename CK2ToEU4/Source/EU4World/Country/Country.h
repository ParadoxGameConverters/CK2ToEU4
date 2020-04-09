#ifndef EU4_COUNTRY_H
#define EU4_COUNTRY_H

#include "../../CK2World/Titles/Title.h"
#include "../../Mappers/LocalizationMapper/LocalizationMapper.h"
#include "CountryDetails.h"
#include <memory>
#include <string>

namespace mappers
{
class GovernmentsMapper;
class ReligionMapper;
class CultureMapper;
class ColorScraper;
class ProvinceMapper;
} // namespace mappers

namespace EU4
{
class Country
{
  public:
	Country() = default;

	Country(std::string theTag, const std::string& filePath);
	void loadHistory(const std::string& filePath);
	void initializeFromTitle(std::string theTag,
		 std::shared_ptr<CK2::Title> theTitle,
		 const mappers::GovernmentsMapper& governmentsMapper,
		 const mappers::ReligionMapper& religionMapper,
		 const mappers::CultureMapper& cultureMapper,
		 const mappers::ProvinceMapper& provinceMapper,
		 const mappers::ColorScraper& colorScraper,
		 const mappers::LocalizationMapper& localizationMapper);

	void outputCommons(std::ostream& output);

	[[nodiscard]] const auto& getCommonCountryFile() const { return commonCountryFile; }
	[[nodiscard]] const auto& getHistoryCountryFile() const { return historyCountryFile; }
	[[nodiscard]] const auto& getLocalizations() const { return localizations; }

	friend std::ostream& operator<<(std::ostream& output, const Country& versionParser);

  private:
	std::string tag;
	std::string commonCountryFile;
	std::string historyCountryFile;
	std::shared_ptr<CK2::Title> title;
	std::map<std::string, mappers::LocBlock> localizations;

	CountryDetails details;
};
} // namespace EU4

#endif // EU4_COUNTRY_H