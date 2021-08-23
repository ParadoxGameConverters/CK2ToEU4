#ifndef INTERNAL_MODIFIERS
#define INTERNAL_MODIFIERS
#include "Parser.h"

namespace mappers
{
class InternalModifiers: commonItems::parser
{
  public:
	InternalModifiers() = default;
	explicit InternalModifiers(std::istream& theStream);

	[[nodiscard]] const auto& getModifierMap() const { return modifierMap; }

  private:
	void registerKeys();

	std::map<std::string, std::vector<double>> modifierMap; //e.g. local_defensiveness, {0.05, 0.2, 0.33, 0.33}
};
} // namespace mappers

#endif // INTERNAL_MODIFIERS_H