#ifndef CULTURE_MAPPING_RULE_H
#define CULTURE_MAPPING_RULE_H

#include "newParser.h"
#include <set>

namespace mappers
{
class RegionMapper;
class CultureMappingRule: commonItems::parser
{
  public:
	CultureMappingRule() = default;
	explicit CultureMappingRule(std::istream& theStream);

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

	void insertRegionMapper(std::shared_ptr<RegionMapper> theRegionMapper) { regionMapper = theRegionMapper; }

	[[nodiscard]] const auto& getEU4Culture() const { return destinationCulture; } // for testing
	[[nodiscard]] const auto& getCK2Cultures() const { return cultures; }			 // for testing
	[[nodiscard]] const auto& getReligions() const { return religions; }				 // for testing
	[[nodiscard]] const auto& getRegions() const { return regions; }					 // for testing
	[[nodiscard]] const auto& getOwners() const { return owners; }						 // for testing
	[[nodiscard]] const auto& getProvinces() const { return provinces; }				 // for testing

  private:
	std::string destinationCulture;
	std::set<std::string> cultures;
	std::set<std::string> religions;
	std::set<std::string> regions;
	std::set<std::string> owners;
	std::set<int> provinces;

	std::shared_ptr<RegionMapper> regionMapper;
};
} // namespace mappers

#endif // CULTURE_MAPPING_RULE_H
