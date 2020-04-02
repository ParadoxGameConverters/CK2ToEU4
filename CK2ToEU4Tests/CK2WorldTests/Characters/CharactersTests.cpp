#include "gtest/gtest.h"
#include <sstream>
#include "../../CK2ToEU4/Source/CK2World/Characters/Characters.h"

TEST(CK2World_CharactersTests, CharactersDefaultToEmpty)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	const CK2::Characters characters(input);

	ASSERT_EQ(characters.getCharacters().empty(), true);
}

TEST(CK2World_CharactersTests, charactersCanBeLoaded)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "42={}\n";
	input << "43={}\n";
	input << "}";

	const CK2::Characters characters(input);
	const auto& characterItr = characters.getCharacters().begin();
	const auto& characterItr2 = --characters.getCharacters().end();

	ASSERT_EQ(characterItr->first, 42);
	ASSERT_EQ(characterItr->second.getID(), 42);
	ASSERT_EQ(characterItr2->first, 43);
	ASSERT_EQ(characterItr2->second.getID(), 43);
}

