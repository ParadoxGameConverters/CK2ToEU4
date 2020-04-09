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
class TitleTagMapper;
}

namespace EU4
{
class Province
{
  public:
	Province() = default;

	Province(int id, const std::string& filePath);
	void initializeFromCK2(std::shared_ptr<CK2::Province> origProvince, mappers::TitleTagMapper& titletagMapper);

	[[nodiscard]] const auto& getHistoryCountryFile() const { return historyProvincesFile; }

	friend std::ostream& operator<<(std::ostream& output, const Province& versionParser);

  private:
	int provID = 0;
	std::string historyProvincesFile;
	std::shared_ptr<CK2::Province> srcProvince;
	ProvinceDetails details;
};
} // namespace EU4

#endif // EU4_PROVINCE_H