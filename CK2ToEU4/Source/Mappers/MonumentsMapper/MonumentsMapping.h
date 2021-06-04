#ifndef MONUMENTS_MAPPING
#define MONUMENTS_MAPPING
#include "Color.h"
#include "Parser.h"
#include <set>



namespace mappers
{
class MonumentsMapping: commonItems::parser
{
  public:
	MonumentsMapping() = default;
	MonumentsMapping(std::istream& theStream);

	void AddProvinceSet(std::istream& theStream);
	void AddAreaSet(std::istream& theStream);
	void AddCountrySet(std::istream& theStream);

	[[nodiscard]] auto getIsBase() { return isBase; }
	[[nodiscard]] auto getProvinceModifiers() { return provinceModifiers; }
	[[nodiscard]] auto getAreaModifiers() { return areaModifiers; }
	[[nodiscard]] auto getCountryModifiers() { return countryModifiers; }	
	[[nodiscard]] auto getOnUpgraded() { return onUpgraded; }	
	[[nodiscard]] auto isOfBuilderCulture() { return cultural; }
	[[nodiscard]] auto isOfBuilderReligion() { return religious; }
	[[nodiscard]] auto getBuildTrigger() { return buildTrigger; }
	void setCanBeMoved(bool mod) { canBeMoved = mod; }
	[[nodiscard]] auto getCanBeMoved() { return canBeMoved; }
	

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