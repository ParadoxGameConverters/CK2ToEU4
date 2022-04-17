#ifndef CULTURE_MAPPER_H
#define CULTURE_MAPPER_H

#include "../RegionMapper/RegionMapper.h"
#include "CultureMappingRule.h"
#include "Parser.h"
#include <optional>
#include <string>

namespace mappers
{
class CultureMapper: commonItems::parser
{
  public:
	CultureMapper() = default;
	void initCultureMapper(const std::string& path);
	void initCultureMapper(std::istream& theStream);

	void loadRegionMapper(std::shared_ptr<RegionMapper> theRegionMapper);

	[[nodiscard]] std::optional<std::string> cultureMatch(const std::string& ck2culture,
		 const std::string& eu4religion,
		 int eu4Province,
		 const std::string& eu4ownerTag) const;

	[[nodiscard]] std::optional<std::string> cultureRegionalMatch(const std::string& ck2culture,
		 const std::string& eu4religion,
		 int eu4Province,
		 const std::string& eu4ownerTag) const;

	[[nodiscard]] std::optional<std::string> cultureNonRegionalNonReligiousMatch(const std::string& ck2culture,
		 const std::string& eu4religion,
		 int eu4Province,
		 const std::string& eu4ownerTag) const;

	[[nodiscard]] std::optional<std::string> getTechGroup(const std::string& incEU4Culture) const;
	[[nodiscard]] std::optional<std::string> getGFX(const std::string& incEU4Culture) const;

  private:
	void registerKeys();

	std::vector<CultureMappingRule> cultureMapRules;
};
} // namespace mappers

#endif // CULTURE_MAPPER_H