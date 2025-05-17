#ifndef REFORMED_RELIGION_MAPPER
#define REFORMED_RELIGION_MAPPER
#include "Parser.h"
#include "ReformedReligionMapping.h"

namespace mappers
{
class ReformedReligionMapper: commonItems::parser
{
  public:
	ReformedReligionMapper() = default;
	void initReformedReligionMapper(std::istream& theStream);
	void initReformedReligionMapper(const std::filesystem::path& path);

	std::map<std::string, ReformedReligionMapping> getReligionEntries() { return religionEntries; };

  private:
	void registerKeys();


	std::map<std::string, ReformedReligionMapping> religionEntries;
};
} // namespace mappers

#endif // REFORMED_RELIGION_MAPPER_H