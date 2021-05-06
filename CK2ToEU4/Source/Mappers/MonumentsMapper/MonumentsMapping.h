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

	void CreateBuildTrigger(std::istream& theStream);
	void AddModifiers(std::istream& theStream);
	void AddProvinceSet(std::istream& theStream);
	void AddAreaSet(std::istream& theStream);
	void AddCountrySet(std::istream& theStream);

	[[nodiscard]] auto getBuildTrigger() { return canBeMoved; }
	void setCanBeMoved(bool mod) { canBeMoved = mod; }
	[[nodiscard]] auto getCanBeMoved() { return buildTrigger; }
	void setBuildTrigger(std::string mod) { buildTrigger = mod; }

  private:
	void registerKeys();

	bool isBase = false;
	bool canBeMoved = false;
	bool cultural = false;
	bool religious = false;

	short numOfModifiers = 0;

	std::map<std::string, std::set<int>> provinceModifiers;
	std::map<std::string, std::set<int>> areaModifiers;
	std::map<std::string, std::set<int>> countryModifiers;

	std::string buildTrigger = "OR = {\n\t\t";
};
} // namespace mappers

#endif // CK2_MONUMENTS_MAPPING_H