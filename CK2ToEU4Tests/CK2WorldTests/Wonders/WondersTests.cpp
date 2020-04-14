#include "gtest/gtest.h"
#include <sstream>
#include "../../CK2ToEU4/Source/CK2World/Wonders/Wonder.h"
#include "../../CK2ToEU4/Source/CK2World/Wonders/Wonders.h"

TEST(CK2World_WondersTests, WondersDefaultToEmpty)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	const CK2::Wonders wonders(input);

	ASSERT_TRUE(wonders.getWonders().empty());
}

TEST(CK2World_WondersTests, WondersCanBeLoaded)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "42={}\n";
	input << "43={}\n";
	input << "}";

	const CK2::Wonders wonders(input);
	const auto& wonder = wonders.getWonders().find(42);
	const auto& wonder2 = wonders.getWonders().find(43);
	
	ASSERT_EQ(wonders.getWonders().size(), 2);
	ASSERT_EQ(wonder->first, 42);
	ASSERT_EQ(wonder2->first, 43);
}
