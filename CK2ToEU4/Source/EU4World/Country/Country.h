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
		 const mappers::LocalizationMapper& localizationMapper,
		 date theConversionDate);
	void initializeRulers(const mappers::ReligionMapper& religionMapper, const mappers::CultureMapper& cultureMapper);

	void outputCommons(std::ostream& output);

	[[nodiscard]] const auto& getCommonCountryFile() const { return commonCountryFile; }
	[[nodiscard]] const auto& getHistoryCountryFile() const { return historyCountryFile; }
	[[nodiscard]] const auto& getLocalizations() const { return localizations; }
	[[nodiscard]] const auto& getPrimaryCulture() const { return details.primaryCulture; }
	[[nodiscard]] const auto& getReligion() const { return details.religion; }
	[[nodiscard]] const auto& getProvinces() const { return provinces; }
	[[nodiscard]] const auto& getTitle() const { return title; }
	[[nodiscard]] auto isHREEmperor() const { return details.holyRomanEmperor; }
	[[nodiscard]] auto isinHRE() const { return details.inHRE; }
	[[nodiscard]] int getDevelopment() const;
	[[nodiscard]] const auto& getGovernment() const { return details.government; }
	[[nodiscard]] const auto& getGovernmentReforms() const { return details.reforms; }
	[[nodiscard]] const auto& getTag() const { return tag; }
	[[nodiscard]] auto getConversionDate() const { return conversionDate; }

	void registerProvince(std::pair<int, std::shared_ptr<Province>> theProvince) { provinces.insert(std::move(theProvince)); }
	void setPrimaryCulture(const std::string& culture);
	void setReligion(const std::string& religion);
	void overrideReforms(const std::string& reform) { details.reforms = { reform }; }
	void setGovernment(const std::string& government) { details.government = government; }
	void setElector() { details.elector = true; }

	friend std::ostream& operator<<(std::ostream& output, const Country& versionParser);

  private:
	std::string tag;
	std::string commonCountryFile;
	std::string historyCountryFile;
	std::pair<std::string, std::shared_ptr<CK2::Title>> title;
	std::map<std::string, mappers::LocBlock> localizations;

	CountryDetails details;
	std::map<int, std::shared_ptr<Province>> provinces;
	date conversionDate; // for dating the monarchs in history file.
};
} // namespace EU4

#endif // EU4_COUNTRY_H