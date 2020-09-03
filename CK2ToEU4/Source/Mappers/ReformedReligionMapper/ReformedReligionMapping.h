#ifndef REFORMED_RELIGION_MAPPING
#define REFORMED_RELIGION_MAPPING
#include "Parser.h"
#include <set>



namespace mappers
{
class ReformedReligionMapping: commonItems::parser
{
  public:
	ReformedReligionMapping();
	ReformedReligionMapping(std::istream& theStream, std::string theFeatures);

	std::string getBeforeCountry() { return beforeCountry; }
	void setBeforeCountry(std::string mod) { beforeCountry = mod; }

	std::string getCountryModifiers() { return countryModifiers; }
	void setCountryModifiers(std::string mod) { countryModifiers = mod; }
	std::string getProvinceModifiers() { return provinceModifiers; }
	void setProvinceModifiers(std::string mod) { provinceModifiers = mod; }
	std::string getUniqueMechanics() { return uniqueMechanics; }
	void setUniqueMechanics(std::string mod) { uniqueMechanics = mod; }
	std::string getNonUniqueMechanics() { return nonUniqueMechanics; }
	void setNonUniqueMechanics(std::string mod) { nonUniqueMechanics = mod; }

	std::string getHereticStrings() { return hereticStrings; }
	void setHereticStrings(std::string mod) { hereticStrings = mod; }

  private:

	std::string beforeCountry;

	std::string countryModifiers;
	std::string provinceModifiers;
	std::string uniqueMechanics;
	std::string nonUniqueMechanics;

	std::string hereticStrings;
	
};
} // namespace CK2

#endif // CK2_RELIGIONS_H