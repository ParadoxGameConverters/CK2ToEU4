#ifndef PROVINCE_MAPPING_H
#define PROVINCE_MAPPING_H

#include "Parser.h"

namespace mappers
{
class ProvinceMapping: commonItems::parser
{
  public:
	ProvinceMapping() = default;
	explicit ProvinceMapping(std::istream& theStream);

	[[nodiscard]] const auto& getEU4Provinces() const { return eu4Provinces; }
	[[nodiscard]] const auto& getCK2Provinces() const { return ck2Provinces; }

  private:
	void registerKeys();

	std::vector<int> eu4Provinces;
	std::vector<int> ck2Provinces;
};
} // namespace mappers

#endif // PROVINCE_MAPPING_H