#include "../../CK2ToEU4/Source/CK2World/Relations/Relations.h"
#include "gtest/gtest.h"
#include <sstream>

TEST(CK2World_RelationsTests, RelationsDefaultToEmpty)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	const CK2::Relations relations(input, "diplo_42");

	ASSERT_TRUE(relations.getRelations().empty());
}

TEST(CK2World_RelationsTests, RelationsCanBeLoaded)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "3 = {}\n";
	input << "4 = {}\n";
	input << "}";

	const CK2::Relations relations(input, "diplo_42");

	ASSERT_FALSE(relations.getRelations().empty());
	ASSERT_EQ(relations.getRelations().size(), 2);
}

TEST(CK2World_RelationsTests, RelationsForwardsSourceCharacterID)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "3 = {}\n";
	input << "4 = {}\n";
	input << "}";

	const CK2::Relations relations(input, "diplo_42");
	const auto& firstRelation = relations.getRelations().front();
	const auto& lastRelation = relations.getRelations().back();

	ASSERT_EQ(firstRelation.getFirst(), 42);
	ASSERT_EQ(lastRelation.getFirst(), 42);
}

TEST(CK2World_RelationsTests, IndividualRelationsMapTargetID)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "3 = {}\n";
	input << "4 = {}\n";
	input << "}";

	const CK2::Relations relations(input, "diplo_42");
	const auto& firstRelation = relations.getRelations().front();
	const auto& lastRelation = relations.getRelations().back();

	ASSERT_EQ(firstRelation.getSecond(), 3);
	ASSERT_EQ(lastRelation.getSecond(), 4);
}
