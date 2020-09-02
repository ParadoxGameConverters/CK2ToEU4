#ifndef REFORMED_RELIGION_MAPPER
#define REFORMED_RELIGION_MAPPER
#include "ReformedReligionMapping.h"
#include "Parser.h"

namespace mappers
{
class ReformedReligionMapper: commonItems::parser
{
  public:
	ReformedReligionMapper();
	explicit ReformedReligionMapper(std::istream& theStream);							 // testing

	std::map<std::string, ReformedReligionMapping> getReligionEntries() { return religionEntries; };

  private:
	void registerKeys();


	std::map<std::string, ReformedReligionMapping> religionEntries;

};
} // namespace mappers

#endif // REFORMED_RELIGION_MAPPER_H