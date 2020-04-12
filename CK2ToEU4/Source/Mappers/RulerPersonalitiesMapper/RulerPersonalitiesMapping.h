#ifndef RULER_PERSONALITIES_MAPPING_H
#define RULER_PERSONALITIES_MAPPING_H

#include "newParser.h"

namespace CK2
{
class Character;
}
namespace mappers
{
class RulerPersonalitiesMapping: commonItems::parser
{
  public:
	RulerPersonalitiesMapping() = default;
	explicit RulerPersonalitiesMapping(std::istream& theStream);

	[[nodiscard]] int evaluatePersonality(const std::pair<int, std::shared_ptr<CK2::Character>>& theCharacter) const;

	[[nodiscard]] const auto& getPersonalities() const { return personalities; } // used for testing

  private:
	void registerKeys();

	std::map<std::string, int> personalities;
};
} // namespace mappers

#endif // RULER_PERSONALITIES_MAPPING_H