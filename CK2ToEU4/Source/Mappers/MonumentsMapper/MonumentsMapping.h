#ifndef MONUMENTS_MAPPING
#define MONUMENTS_MAPPING
#include "Parser.h"

namespace mappers
{
class MonumentsMapping: commonItems::parser
{
  public:
	MonumentsMapping() = default;
	explicit MonumentsMapping(std::istream& theStream);

	void AddProvinceSet(std::istream& theStream);
	void AddAreaSet(std::istream& theStream);
	void AddCountrySet(std::istream& theStream);

	[[nodiscard]] auto getIsBase() const { return isBase; }
	[[nodiscard]] auto getProvinceModifiers() const { return provinceModifiers; }
	[[nodiscard]] auto getAreaModifiers() const { return areaModifiers; }
	[[nodiscard]] auto getCountryModifiers() const { return countryModifiers; }
	[[nodiscard]] auto getOnUpgraded() const { return onUpgraded; }
	[[nodiscard]] auto isOfBuilderCulture() const { return cultural; }
	[[nodiscard]] auto isOfBuilderReligion() const { return religious; }
	[[nodiscard]] auto getBuildTrigger() const { return buildTrigger; }
	[[nodiscard]] auto getCanBeMoved() const { return canBeMoved; }


  private:
	void registerKeys();

	bool isBase = false;
	bool canBeMoved = false;
	bool cultural = false;
	bool religious = false;

	// Modifier, { tier0, tier1, tier2, tier3 }
	std::map<std::string, std::vector<double>> provinceModifiers;
	std::map<std::string, std::vector<double>> areaModifiers;
	std::map<std::string, std::vector<double>> countryModifiers;

	std::string buildTrigger;
	std::string onUpgraded;
};
} // namespace mappers

#endif // CK2_MONUMENTS_MAPPING_H