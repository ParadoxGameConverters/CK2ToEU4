#ifndef REFORMED_RELIGION_MAPPING
#define REFORMED_RELIGION_MAPPING
#include "Parser.h"
#include "Color.h"
#include <set>



namespace mappers
{
class ReformedReligionMapping: commonItems::parser
{
  public:
	ReformedReligionMapping() = default;
	ReformedReligionMapping(std::istream& theStream);

	[[nodiscard]] const auto& getName() { return name; }
	void setName(std::string mod) { name = mod; }
	[[nodiscard]] short getIconNumber() { return iconNumber; }
	void setIconNumber(int mod) { iconNumber = mod; }
	[[nodiscard]] std::optional<commonItems::Color> getColor() { return color; }
	void setColor(std::optional<commonItems::Color> mod) { color = mod; }

	[[nodiscard]] const auto& getCountryModifiers() { return countryModifiers; }
	void setCountryModifiers(std::string mod) { countryModifiers = mod; }
	void addCountryModifiers(std::string mod);
	[[nodiscard]] const auto& getProvinceModifiers() { return provinceModifiers; }
	void setProvinceModifiers(std::string mod) { provinceModifiers = mod; }
	void addProvinceModifiers(std::string mod);
	[[nodiscard]] const auto& getSecondary() { return secondary; }
	void setSecondary(std::string mod) { secondary = mod; }
	void addSecondary(std::string mod);
	[[nodiscard]] const auto& getUniqueMechanics() { return uniqueMechanics; }
	void setUniqueMechanics(std::string mod) { uniqueMechanics = mod; }
	[[nodiscard]] const auto& getNonUniqueMechanics() { return nonUniqueMechanics; }
	void setNonUniqueMechanics(std::string mod) { nonUniqueMechanics = mod; }
	void addNonUniqueMechanics(std::string mod);

	[[nodiscard]] const auto& getHereticStrings() { return hereticStrings; }
	void setHereticStrings(std::string mod) { hereticStrings = mod; }

  private:
	void registerKeys();

	std::string name;
	int iconNumber = 0;
	std::optional<commonItems::Color> color;

	std::string countryModifiers;
	std::string provinceModifiers;
	std::string secondary;
	std::string uniqueMechanics;
	std::string nonUniqueMechanics;

	std::string hereticStrings;
	
};
} // namespace CK2

#endif // CK2_RELIGIONS_H