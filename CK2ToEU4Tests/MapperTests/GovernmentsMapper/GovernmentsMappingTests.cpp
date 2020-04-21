#include "../../CK2ToEU4/Source/Mappers/GovernmentsMapper/GovernmentsMapping.h"
#include "gtest/gtest.h"
#include <sstream>


TEST(Mappers_GovernmentsMappingTests, eu4GovernmentDefaultsToBlank)
{
	std::stringstream input;
	input << "= {}";

	const mappers::GovernmentsMapping theMapping(input);

	ASSERT_TRUE(theMapping.getGovernment().empty());
}

TEST(Mappers_GovernmentsMappingTests, eu4GovernmentCanBeSet)
{
	std::stringstream input;
	input << "= { gov = eu4Government }";

	const mappers::GovernmentsMapping theMapping(input);

	ASSERT_EQ(theMapping.getGovernment(), "eu4Government");
}

TEST(Mappers_GovernmentsMappingTests, ck2GovernmentsDefaultToEmpty)
{
	std::stringstream input;
	input << "= {}";

	const mappers::GovernmentsMapping theMapping(input);

	ASSERT_TRUE(theMapping.getCK2Governments().empty());
}

TEST(Mappers_GovernmentsMappingTests, ck2GovernmentsCanBeSet)
{
	std::stringstream input;
	input << "= { ck2gov = religion1 ck2gov = religion2 }";

	const mappers::GovernmentsMapping theMapping(input);

	ASSERT_EQ(theMapping.getCK2Governments().size(), 2);
	ASSERT_EQ(*theMapping.getCK2Governments().find("religion1"), "religion1");
	ASSERT_EQ(*theMapping.getCK2Governments().find("religion2"), "religion2");
}

TEST(Mappers_GovernmentsMappingTests, ck2titleDefaultToEmpty)
{
	std::stringstream input;
	input << "= {}";

	const mappers::GovernmentsMapping theMapping(input);

	ASSERT_TRUE(theMapping.getCK2Title().empty());
}

TEST(Mappers_GovernmentsMappingTests, ck2TitleCanBeSet)
{
	std::stringstream input;
	input << "= { ck2title = c_test }";

	const mappers::GovernmentsMapping theMapping(input);

	ASSERT_EQ(theMapping.getCK2Title(), "c_test");
}

TEST(Mappers_GovernmentsMappingTests, reformDefaultToEmpty)
{
	std::stringstream input;
	input << "= {}";

	const mappers::GovernmentsMapping theMapping(input);

	ASSERT_TRUE(theMapping.getReform().empty());
}

TEST(Mappers_GovernmentsMappingTests, reformCanBeSet)
{
	std::stringstream input;
	input << "= { reform = theReform }";

	const mappers::GovernmentsMapping theMapping(input);

	ASSERT_EQ(theMapping.getReform(), "theReform");
}