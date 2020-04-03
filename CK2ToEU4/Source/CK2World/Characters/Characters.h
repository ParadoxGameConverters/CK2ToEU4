#ifndef CK2_CHARACTERS_H
#define CK2_CHARACTERS_H
#include "newParser.h"

namespace CK2
{
	class Character;
	class Characters: commonItems::parser
	{
	public:
		Characters() = default;
		Characters(std::istream& theStream);
		[[nodiscard]] const auto& getCharacters() const { return characters; }
				
	private:
		void registerKeys();

		std::map<int, std::shared_ptr<Character>> characters;
	};
}

#endif // CK2_CHARACTERS_H
