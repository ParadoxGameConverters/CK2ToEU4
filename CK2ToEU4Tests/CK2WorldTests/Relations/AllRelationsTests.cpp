#include "../../CK2ToEU4/Source/CK2World/Relations/AllRelations.h"
#include "gtest/gtest.h"
#include <sstream>

TEST(CK2World_DiplomacyTests, DiplomacyDefaultsToEmpty)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	const CK2::Diplomacy diplomacy(input);

	ASSERT_TRUE(diplomacy.getDiplomacy().empty());
}

TEST(CK2World_DiplomacyTests, DiplomacyCanBeLoaded)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "diplo_7 = { 3 = {} 8 = {} 11 = {}}\n";
	input << "diplo_8 = { 11 = {} }\n";
	input << "}";

	const CK2::Diplomacy diplomacy(input);
	const auto& diplo7 = diplomacy.getDiplomacy().find(7);

	ASSERT_FALSE(diplomacy.getDiplomacy().empty());
	ASSERT_EQ(diplomacy.getDiplomacy().size(), 2);
	ASSERT_EQ(diplo7->second.getRelations().size(), 3);
}
