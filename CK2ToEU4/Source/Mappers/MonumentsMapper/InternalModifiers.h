#ifndef INTERNAL_MODIFIERS
#define INTERNAL_MODIFIERS
#include "Parser.h"

namespace mappers
{
	class InternalModifiers: commonItems::parser
	{
	  public:
		InternalModifiers();
		InternalModifiers(std::istream& theStream);

		[[nodiscard]] auto getModifierType() { return modifierType; }
		[[nodiscard]] auto getModifierValues() { return modifierValues; }

	  private:
		void registerKeys();

		std::string modifierType; //e.g. local_defensiveness
		std::vector<double> modifierValues; //e.g. {0.05, 0.2, 0.33, 0.33}		
	};
} // namespace mappers

#endif // INTERNAL_MODIFIERS_H