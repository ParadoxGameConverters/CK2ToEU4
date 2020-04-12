#ifndef EU4_PROVINCE_H
#define EU4_PROVINCE_H

#include "ProvinceDetails.h"
#include <memory>
#include <string>

namespace CK2
{
class Province;
}
namespace mappers
{
class CultureMapper;
class ReligionMapper;
} // namespace mappers

namespace EU4
{
class Country;
class Province
{
  public:
	Province() = default;

	Province(int id, const std::string& filePath);
	void initializeFromCK2(std::shared_ptr<CK2::Province> origProvince, const mappers::CultureMapper& cultureMapper, const mappers::ReligionMapper& religionMapper);

	[[nodiscard]] const auto& getHistoryCountryFile() const { return historyProvincesFile; }
	[[nodiscard]] const auto& getTagCountry() const { return tagCountry; }
	[[nodiscard]] const auto& getOwner() const { return details.owner; }
	[[nodiscard]] const auto& getReligion() const { return details.religion; }
	[[nodiscard]] const auto& getCulture() const { return details.culture; }
	[[nodiscard]] auto getDev() const { return details.baseTax + details.baseProduction + details.baseManpower; }

	void registerTagCountry(const std::pair<std::string, std::shared_ptr<Country>>& theCountry) { tagCountry = theCountry; }
	void addCore(const std::string& core) { details.cores.insert(core); }
	void setOwner(const std::string& tag) { details.owner = tag; }
	void setController(const std::string& tag) { details.controller = tag; }

	friend std::ostream& operator<<(std::ostream& output, const Province& versionParser);

  private:
	int provID = 0;
	std::string historyProvincesFile;
	std::shared_ptr<CK2::Province> srcProvince;
	ProvinceDetails details;
	std::pair<std::string, std::shared_ptr<Country>> tagCountry;
};
} // namespace EU4

#endif // EU4_PROVINCE_H