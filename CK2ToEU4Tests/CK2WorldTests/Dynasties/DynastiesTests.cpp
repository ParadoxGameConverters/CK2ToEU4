#include "../../CK2ToEU4/Source/CK2World/Dynasties/Dynasties.h"
#include "../../CK2ToEU4/Source/CK2World/Dynasties/Dynasty.h"
#include "gtest/gtest.h"
#include <sstream>

TEST(CK2World_DynastiesTests, DynastiesDefaultToEmpty)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	const CK2::Dynasties dynasties(input);

	ASSERT_EQ(dynasties.getDynasties().empty(), true);
}

TEST(CK2World_DynastiesTests, dynastiesCanBeLoaded)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "42={}\n";
	input << "43={}\n";
	input << "}";

	const CK2::Dynasties dynasties(input);
	const auto& characterItr = dynasties.getDynasties().find(42);
	const auto& characterItr2 = dynasties.getDynasties().find(43);

	ASSERT_EQ(characterItr->first, 42);
	ASSERT_EQ(characterItr->second->getID(), 42);
	ASSERT_EQ(characterItr2->first, 43);
	ASSERT_EQ(characterItr2->second->getID(), 43);
}
