#include "gtest/gtest.h"
#include "../../CK2ToEU4/Source/Mappers/CultureMapper/CultureMappingRule.h"
#include <sstream>

TEST(Mappers_CultureMappingTests, eu4CultureDefaultsToBlank)
{
	std::stringstream input;
	input << "= {}";

	const mappers::CultureMappingRule theMapping(input);

	ASSERT_TRUE(theMapping.getEU4Culture().empty());
}

TEST(Mappers_CultureMappingTests, eu4CultureCanBeSet)
{
	std::stringstream input;
	input << "= { eu4 = eu4Culture }";

	const mappers::CultureMappingRule theMapping(input);

	ASSERT_EQ(theMapping.getEU4Culture(), "eu4Culture");
}

TEST(Mappers_CultureMappingTests, ck2CulturesDefaultToEmpty)
{
	std::stringstream input;
	input << "= {}";

	const mappers::CultureMappingRule theMapping(input);

	ASSERT_TRUE(theMapping.getCK2Cultures().empty());
}

TEST(Mappers_CultureMappingTests, ck2CulturesCanBeSet)
{
	std::stringstream input;
	input << "= { ck2 = culture1 ck2 = culture2 }";

	const mappers::CultureMappingRule theMapping(input);

	ASSERT_EQ(theMapping.getCK2Cultures().size(), 2);
	ASSERT_EQ(*theMapping.getCK2Cultures().find("culture1"), "culture1");
	ASSERT_EQ(*theMapping.getCK2Cultures().find("culture2"), "culture2");
}

TEST(Mappers_CultureMappingTests, ReligionsDefaultToEmpty)
{
	std::stringstream input;
	input << "= {}";

	const mappers::CultureMappingRule theMapping(input);

	ASSERT_TRUE(theMapping.getReligions().empty());
}

TEST(Mappers_CultureMappingTests, ReligionsCanBeSet)
{
	std::stringstream input;
	input << "= { religion = religion1 religion = religion2 }";

	const mappers::CultureMappingRule theMapping(input);

	ASSERT_EQ(theMapping.getReligions().size(), 2);
	ASSERT_EQ(*theMapping.getReligions().find("religion1"), "religion1");
	ASSERT_EQ(*theMapping.getReligions().find("religion2"), "religion2");
}


TEST(Mappers_CultureMappingTests, RegionsDefaultToEmpty)
{
	std::stringstream input;
	input << "= {}";

	const mappers::CultureMappingRule theMapping(input);

	ASSERT_TRUE(theMapping.getReligions().empty());
}

TEST(Mappers_CultureMappingTests, RegionsCanBeSet)
{
	std::stringstream input;
	input << "= { region = region1 region = region2 }";

	const mappers::CultureMappingRule theMapping(input);

	ASSERT_EQ(theMapping.getRegions().size(), 2);
	ASSERT_EQ(*theMapping.getRegions().find("region1"), "region1");
	ASSERT_EQ(*theMapping.getRegions().find("region2"), "region2");
}

TEST(Mappers_CultureMappingTests, OwnersDefaultToEmpty)
{
	std::stringstream input;
	input << "= {}";

	const mappers::CultureMappingRule theMapping(input);

	ASSERT_TRUE(theMapping.getOwners().empty());
}

TEST(Mappers_CultureMappingTests, OwnersCanBeSet)
{
	std::stringstream input;
	input << "= { owner = TAG1 owner = TAG2 }";

	const mappers::CultureMappingRule theMapping(input);

	ASSERT_EQ(theMapping.getOwners().size(), 2);
	ASSERT_EQ(*theMapping.getOwners().find("TAG1"), "TAG1");
	ASSERT_EQ(*theMapping.getOwners().find("TAG2"), "TAG2");
}

TEST(Mappers_CultureMappingTests, ProvincesDefaultToEmpty)
{
	std::stringstream input;
	input << "= {}";

	const mappers::CultureMappingRule theMapping(input);

	ASSERT_TRUE(theMapping.getProvinces().empty());
}

TEST(Mappers_CultureMappingTests, ProvincesCanBeSet)
{
	std::stringstream input;
	input << "= { provinceid = 3 provinceid = 4 }";

	const mappers::CultureMappingRule theMapping(input);

	ASSERT_EQ(theMapping.getProvinces().size(), 2);
	ASSERT_EQ(*theMapping.getProvinces().find(3), 3);
	ASSERT_EQ(*theMapping.getProvinces().find(4), 4);
}
