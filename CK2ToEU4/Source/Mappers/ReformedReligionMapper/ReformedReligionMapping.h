#ifndef REFORMED_RELIGION_MAPPING
#define REFORMED_RELIGION_MAPPING
#include "Parser.h"
#include <set>



namespace mappers
{
class ReformedReligionMapping: commonItems::parser
{
  public:
	ReformedReligionMapping() = default;
	ReformedReligionMapping(std::istream& theStream);

	std::string getName() { return name; }
	void setName(std::string mod) { name = mod; }
	short getIconNumber() { return iconNumber; }
	void setIconNumber(short mod) { iconNumber = mod; }
	std::string getColor() { return color; }
	void setColor(std::string mod) { color = mod; }

	std::string getCountryModifiers() { return countryModifiers; }
	void setCountryModifiers(std::string mod) { countryModifiers = mod; }
	void addCountryModifiers(std::string mod);
	std::string getProvinceModifiers() { return provinceModifiers; }
	void setProvinceModifiers(std::string mod) { provinceModifiers = mod; }
	void addProvinceModifiers(std::string mod);
	std::string getUniqueMechanics() { return uniqueMechanics; }
	void setUniqueMechanics(std::string mod) { uniqueMechanics = mod; }
	std::string getNonUniqueMechanics() { return nonUniqueMechanics; }
	void setNonUniqueMechanics(std::string mod) { nonUniqueMechanics = mod; }
	void addNonUniqueMechanics(std::string mod);

	std::string getHereticStrings() { return hereticStrings; }
	void setHereticStrings(std::string mod) { hereticStrings = mod; }

  private:
	void registerKeys();

	std::string name;
	short iconNumber;
	std::string color;

	std::string countryModifiers;
	std::string provinceModifiers;
	std::string uniqueMechanics;
	std::string nonUniqueMechanics;

	std::string hereticStrings;
	
};
} // namespace CK2

#endif // CK2_RELIGIONS_H