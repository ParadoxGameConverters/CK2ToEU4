#include "../CK2ToEU4/Source/CK2World/Relations/Relation.h"
#include "gtest/gtest.h"
#include <sstream>

TEST(CK2World_RelationTests, TributaryStatusDefaultsToFalse)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	const CK2::Relation relation(input, 42);

	ASSERT_FALSE(relation.isFirstOverlord());
	ASSERT_FALSE(relation.isSecondOverlord());
}

TEST(CK2World_RelationTests, TributaryStatusIsFalseForEmptyTributary)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "tributary = {}\n";
	input << "}";

	const CK2::Relation relation(input, 42);

	ASSERT_FALSE(relation.isFirstOverlord());
	ASSERT_FALSE(relation.isSecondOverlord());
}

TEST(CK2World_RelationTests, OverlordStatusIsDetectedForFirst)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "tributary = { tributary_type = default tributary = 11}\n";
	input << "}";

	CK2::Relation relation(input, 42);
	relation.setFirst(11);

	ASSERT_FALSE(relation.isFirstOverlord());
	ASSERT_TRUE(relation.isSecondOverlord());
}

TEST(CK2World_RelationTests, OverlordStatusIsDetectedForSecond)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "tributary = { tributary_type = default tributary = 42}\n";
	input << "}";

	CK2::Relation relation(input, 42);
	relation.setFirst(11);

	ASSERT_TRUE(relation.isFirstOverlord());
	ASSERT_FALSE(relation.isSecondOverlord());
}
