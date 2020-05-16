#ifndef RELIGION_MAPPING_H
#define RELIGION_MAPPING_H

#include "Parser.h"
#include <set>

namespace mappers
{
class ReligionMapping: commonItems::parser
{
  public:
	explicit ReligionMapping(std::istream& theStream);

	[[nodiscard]] const auto& getCK2Religions() const { return ck2Religions; }
	[[nodiscard]] const auto& getEU4Religion() const { return eu4Religion; }

  private:
	std::set<std::string> ck2Religions;
	std::string eu4Religion;
};
} // namespace mappers

#endif // RELIGION_MAPPING_H