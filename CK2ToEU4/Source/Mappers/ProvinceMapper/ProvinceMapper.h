#ifndef PROVINCE_MAPPER_H
#define PROVINCE_MAPPER_H

#include "ProvinceMappingsVersion.h"
#include "Parser.h"
#include <map>
#include <set>

class Configuration;

namespace mappers
{
class ProvinceMapper: commonItems::parser
{
  public:
	ProvinceMapper();
	explicit ProvinceMapper(std::istream& theStream);

	[[nodiscard]] std::vector<int> getCK2ProvinceNumbers(int eu4ProvinceNumber) const;
	[[nodiscard]] std::vector<int> getEU4ProvinceNumbers(int ck2ProvinceNumber) const;
	[[nodiscard]] const auto& getOffmapChineseProvinces() const { return offmapChineseProvinces; }
	[[nodiscard]] auto isValidEU4Province(const int eu4Province) const { return validEU4Provinces.count(eu4Province) > 0; }

	void determineValidProvinces(const Configuration& theConfiguration);
	void loadOffmapChineseProvinces();
	void loadOffmapChineseProvinces(std::istream& theStream); // testing

  private:
	void registerKeys();
	void registerOffmapKeys();
	void createMappings();

	std::map<int, std::vector<int>> CK2ToEU4ProvinceMap;
	std::map<int, std::vector<int>> EU4ToCK2ProvinceMap;
	std::set<int> validEU4Provinces;
	std::set<int> offmapChineseProvinces;
	ProvinceMappingsVersion theMappings;
};
} // namespace mappers

#endif // PROVINCE_MAPPER_H