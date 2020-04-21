#include "../../CK2ToEU4/Source/Mappers/ReligionMapper/ReligionMapping.h"
#include "gtest/gtest.h"
#include <sstream>


TEST(Mappers_ReligionMappingTests, eu4ReligionDefaultsToBlank)
{
	std::stringstream input;
	input << "= {}";

	const mappers::ReligionMapping theMapping(input);

	ASSERT_TRUE(theMapping.getEU4Religion().empty());
}


TEST(Mappers_ReligionMappingTests, eu4ReligionCanBeSet)
{
	std::stringstream input;
	input << "= { eu4 = eu4Religion }";

	const mappers::ReligionMapping theMapping(input);

	ASSERT_EQ(theMapping.getEU4Religion(), "eu4Religion");
}


TEST(Mappers_ReligionMappingTests, ck2ReligionsDefaultToEmpty)
{
	std::stringstream input;
	input << "= {}";

	const mappers::ReligionMapping theMapping(input);

	ASSERT_TRUE(theMapping.getCK2Religions().empty());
}


TEST(Mappers_ReligionMappingTests, ck2ReligionsCanBeSet)
{
	std::stringstream input;
	input << "= { ck2 = religion1 ck2 = religion2 }";

	const mappers::ReligionMapping theMapping(input);

	ASSERT_EQ(theMapping.getCK2Religions().size(), 2);
	ASSERT_EQ(*theMapping.getCK2Religions().find("religion1"), "religion1");
	ASSERT_EQ(*theMapping.getCK2Religions().find("religion2"), "religion2");
}