#ifndef CULTURE_MAPPER_H
#define CULTURE_MAPPER_H

#include "../RegionMapper/RegionMapper.h"
#include "CultureMappingRule.h"
#include "newParser.h"
#include <optional>
#include <string>

namespace mappers
{
class CultureMapper: commonItems::parser
{
  public:
	CultureMapper();
	explicit CultureMapper(std::istream& theStream);

	void loadRegionMapper(std::shared_ptr<RegionMapper> theRegionMapper);

	[[nodiscard]] std::optional<std::string> cultureMatch(const std::string& eu4culture,
		 const std::string& eu4religion,
		 int eu4Province,
		 const std::string& eu4ownerTag) const;

	[[nodiscard]] std::optional<std::string> cultureRegionalMatch(const std::string& eu4culture,
		 const std::string& eu4religion,
		 int eu4Province,
		 const std::string& eu4ownerTag) const;

	[[nodiscard]] std::optional<std::string> cultureNonRegionalNonReligiousMatch(const std::string& eu4culture,
		 const std::string& eu4religion,
		 int eu4Province,
		 const std::string& eu4ownerTag) const;

  private:
	void registerKeys();

	std::vector<CultureMappingRule> cultureMapRules;
};
} // namespace mappers

#endif // CULTURE_MAPPER_H