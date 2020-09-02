#ifndef REFORMED_RELIGION_MAPPING
#define REFORMED_RELIGION_MAPPING
#include "Parser.h"
#include <set>



namespace mappers
{
class ReformedReligionMapping: commonItems::parser
{
  public:
	ReformedReligionMapping(std::istream& theStream, std::string theFeatures);

	std::string getCountryModifiers() { return countryModifiers; };
	std::string getProvinceModifiers() { return provinceModifiers; };
	std::string getUniqueMechanics() { return uniqueMechanics; };
	std::string getNonUniqueMechanics() { return nonUniqueMechanics; };

  private:

	std::string countryModifiers;
	std::string provinceModifiers;
	std::string uniqueMechanics;
	std::string nonUniqueMechanics;
	
};
} // namespace CK2

#endif // CK2_RELIGIONS_H