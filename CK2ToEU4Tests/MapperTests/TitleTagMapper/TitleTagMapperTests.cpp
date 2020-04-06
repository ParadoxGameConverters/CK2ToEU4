#include "gtest/gtest.h"
#include "../CK2ToEU4/Source/Mappers/TitleTagMapper/TitleTagMapper.h"
#include <sstream>

TEST(Mappers_TitleTagMapperTests, emptyMappingsDefaultToEmpty)
{
	std::stringstream input;

	const mappers::TitleTagMapper theMapper(input);

	ASSERT_TRUE(theMapper.getMappings().empty());
	ASSERT_TRUE(theMapper.getRegisteredTitleTags().empty());
}

TEST(Mappers_TitleTagMapperTests, canLoadTitleTagLinks)
{
	std::stringstream input;
	input << "link = { eu4 = TST ck2 = c_test capitals = { 1 2 } }\n";
	input << "link = { eu4 = TST2 ck2 = c_test2 capitals = { 3 4 } }";

	const mappers::TitleTagMapper theMapper(input);

	ASSERT_EQ(theMapper.getMappings().size(), 2);
}

TEST(Mappers_TitleTagMapperTests, canMatchTagsOnTitles)
{
	std::stringstream input;
	input << "link = { eu4 = TST ck2 = c_test capitals = { 1 2 } }\n";
	input << "link = { eu4 = TST2 ck2 = c_test2 capitals = { 3 4 } }";

	mappers::TitleTagMapper theMapper(input);
	const auto& match = theMapper.getTagForTitle("c_test2");

	ASSERT_EQ(*match, "TST2");
}

TEST(Mappers_TitleTagMapperTests, canMatchTagsOnTitlesAndCapitals)
{
	std::stringstream input;
	input << "link = { eu4 = TST ck2 = c_test capitals = { 1 2 } }\n";
	input << "link = { eu4 = TST2 ck2 = c_test2 capitals = { 3 4 } }";

	mappers::TitleTagMapper theMapper(input);
	const auto& match = theMapper.getTagForTitle("c_test2", 3);

	ASSERT_EQ(*match, "TST2");
}

TEST(Mappers_TitleTagMapperTests, canMatchTagsOnTitlesAndBaseTitles)
{
	std::stringstream input;
	input << "link = { eu4 = TST ck2 = c_test capitals = { 1 2 } }\n";
	input << "link = { eu4 = TST2 ck2 = c_test2 capitals = { 3 4 } }";

	mappers::TitleTagMapper theMapper(input);
	const auto& match = theMapper.getTagForTitle("c_dyn_34562", "c_test2", 0);

	ASSERT_EQ(*match, "TST2");
}

TEST(Mappers_TitleTagMapperTests, canGenerateNewTagsForMismatches)
{
	std::stringstream input;
	input << "link = { eu4 = TST ck2 = c_test capitals = { 1 2 } }\n";
	input << "link = { eu4 = TST2 ck2 = c_test2 capitals = { 3 4 } }";

	mappers::TitleTagMapper theMapper(input);
	const auto& match = theMapper.getTagForTitle("c_dyn_34562", 0);
	const auto& match2 = theMapper.getTagForTitle("c_dyn_345622", 0);
	const auto& match3 = theMapper.getTagForTitle("c_dyn_3456233", 0);

	ASSERT_EQ(*match, "Z00");
	ASSERT_EQ(*match2, "Z01");
	ASSERT_EQ(*match3, "Z02");
}

TEST(Mappers_TitleTagMapperTests, canRegisterAllAccessedTitles)
{
	std::stringstream input;
	input << "link = { eu4 = TST ck2 = c_test capitals = { 1 2 } }\n";
	input << "link = { eu4 = TST2 ck2 = c_test2 capitals = { 3 4 } }";

	mappers::TitleTagMapper theMapper(input);
	const auto& match = theMapper.getTagForTitle("c_dyn_34562");
	const auto& match2 = theMapper.getTagForTitle("c_dyn_345622");
	const auto& match3 = theMapper.getTagForTitle("c_dyn_11", 2);
	const auto& match4 = theMapper.getTagForTitle("c_dyn_34262", 0);
	const auto& match5 = theMapper.getTagForTitle("c_test2", "k_nonsense", 11);
	const auto& match6 = theMapper.getTagForTitle("c_dyn_3456233", "d_nonsense", 34);

	ASSERT_EQ(theMapper.getRegisteredTitleTags().size(), 6);
	ASSERT_EQ(*match, "Z00");
	ASSERT_EQ(*match2, "Z01");
	ASSERT_EQ(*match3, "TST");
	ASSERT_EQ(*match4, "Z02");
	ASSERT_EQ(*match5, "TST2");
	ASSERT_EQ(*match6, "Z03");
}
