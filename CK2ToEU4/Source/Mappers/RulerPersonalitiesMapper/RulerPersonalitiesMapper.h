#ifndef RULER_PERSONALITY_MAPPER_H
#define RULER_PERSONALITY_MAPPER_H

#include "Parser.h"
#include "RulerPersonalitiesMapping.h"
#include <set>

namespace CK2
{
class Character;
}

namespace mappers
{
class RulerPersonalitiesMapper: commonItems::parser
{
  public:
	RulerPersonalitiesMapper() = default;
	void initRulerPersonalitiesMapper(std::istream& theStream);
	void initRulerPersonalitiesMapper(const std::string& path);

	[[nodiscard]] std::set<std::string> evaluatePersonalities(const std::pair<int, std::shared_ptr<CK2::Character>>& theCharacter) const;
	[[nodiscard]] const auto& getMappings() const { return theMappings; } // used for testing

  private:
	void registerKeys();

	std::map<std::string, RulerPersonalitiesMapping> theMappings;
};
} // namespace mappers

#endif // RULER_PERSONALITY_MAPPER_H