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

	[[nodiscard]] const auto& getModifierType() const { return modifierType; }
	[[nodiscard]] const auto& getModifierValues() const { return modifierValues; }

  private:
	void registerKeys();

	std::string modifierType;				// e.g. local_defensiveness
	std::vector<double> modifierValues; // e.g. {0.05, 0.2, 0.33, 0.33}
};
} // namespace mappers

#endif // INTERNAL_MODIFIERS_H