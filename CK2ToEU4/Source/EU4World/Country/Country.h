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
class Province;
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
	[[nodiscard]] const auto& getPrimaryCulture() const { return details.primaryCulture; }
	[[nodiscard]] const auto& getReligion() const { return details.religion; }
	[[nodiscard]] const auto& getProvinces() const { return provinces; }
	[[nodiscard]] const auto& getTitle() const { return title; }

	void registerProvince(const std::pair<int, std::shared_ptr<Province>> theProvince) { provinces.insert(theProvince); }
	void setPrimaryCulture(const std::string& culture) { details.primaryCulture = culture; }
	void setReligion(const std::string& religion) { details.religion = religion; }
	
	friend std::ostream& operator<<(std::ostream& output, const Country& versionParser);

  private:
	std::string tag;
	std::string commonCountryFile;
	std::string historyCountryFile;
	std::pair<std::string, std::shared_ptr<CK2::Title>> title;
	std::map<std::string, mappers::LocBlock> localizations;

	CountryDetails details;
	std::map<int, std::shared_ptr<Province>> provinces;
};
} // namespace EU4

#endif // EU4_COUNTRY_H