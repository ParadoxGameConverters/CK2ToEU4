#ifndef RELIGION_MAPPER_H
#define RELIGION_MAPPER_H

#include "Parser.h"
#include <map>
#include <optional>
#include <string>

namespace mappers
{
class ReligionMapper: commonItems::parser
{
  public:
	ReligionMapper() = default;
	void initReligionMapper(std::istream& theStream);
	void initReligionMapper(const std::filesystem::path& path);

	[[nodiscard]] std::optional<std::string> getEu4ReligionForCk2Religion(const std::string& ck2Religion) const;

  private:
	void registerKeys();

	std::map<std::string, std::string> ck2ToEu4ReligionMap;
};
} // namespace mappers

#endif // RELIGION_MAPPER_H
