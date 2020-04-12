#ifndef RULER_PERSONALITY_MAPPER_H
#define RULER_PERSONALITY_MAPPER_H

#include "RulerPersonalitiesMapping.h"
#include "newParser.h"
#include <set>

namespace mappers
{
class RulerPersonalitiesMapper: commonItems::parser
{
  public:
	RulerPersonalitiesMapper();
	explicit RulerPersonalitiesMapper(std::istream& theStream);

	std::set<std::string> getPersonalities(const std::pair<int, std::shared_ptr<CK2::Character>>& theCharacter) const;

	[[nodiscard]] const auto& getMappings() const { return theMappings; } // used for testing

  private:
	void registerKeys();

	std::map<std::string, RulerPersonalitiesMapping> theMappings;
};
} // namespace mappers

#endif // RULER_PERSONALITY_MAPPER_H