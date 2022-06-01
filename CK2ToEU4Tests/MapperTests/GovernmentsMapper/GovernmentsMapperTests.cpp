#include "../../CK2ToEU4/Source/Mappers/GovernmentsMapper/GovernmentsMapper.h"
#include "gtest/gtest.h"
#include <sstream>

TEST(Mappers_GovernmentsMapperTests, nonMatchGivesEmptyOptional)
{
	std::stringstream input;
	input << "link = { gov = eu4Government ck2gov = ck2Government }";

	mappers::GovernmentsMapper theMapper;
	theMapper.initGovernmentsMapper(input);

	ASSERT_FALSE(theMapper.matchGovernment("nonMatchingGovernment", "ck2title"));
}


TEST(Mappers_GovernmentsMapperTests, eu4GovernmentCanBeFound)
{
	std::stringstream input;
	input << "link = { gov = eu4Government ck2gov = ck2Government }";

	mappers::GovernmentsMapper theMapper;
	theMapper.initGovernmentsMapper(input);
	auto match = theMapper.matchGovernment("ck2Government", "ck2title");

	ASSERT_EQ(match->first, "eu4Government");
}


TEST(Mappers_GovernmentsMapperTests, multipleCK2GovernmentsCanBeInARule)
{
	std::stringstream input;
	input << "link = { gov = eu4Government ck2gov = ck2Government ck2gov = ck2Government2 }";

	mappers::GovernmentsMapper theMapper;
	theMapper.initGovernmentsMapper(input);
	auto match = theMapper.matchGovernment("ck2Government2", "ck2title");

	ASSERT_EQ(match->first, "eu4Government");
}

TEST(Mappers_GovernmentsMapperTests, correctRuleMatches)
{
	std::stringstream input;
	input << "link = { gov = eu4Government ck2gov = ck2Government }\n";
	input << "link = { gov = eu4Government2 ck2gov = ck2Government2 }";

	mappers::GovernmentsMapper theMapper;
	theMapper.initGovernmentsMapper(input);
	auto match = theMapper.matchGovernment("ck2Government2", "ck2title");

	ASSERT_EQ(match->first, "eu4Government2");
}

TEST(Mappers_GovernmentsMapperTests, ck2TitleCanBeFound)
{
	std::stringstream input;
	input << "link = { gov = eu4Government ck2gov = ck2Government }\n";
	input << "link = { gov = eu4Government2 ck2gov = ck2Government2 ck2title = c_test }";

	mappers::GovernmentsMapper theMapper;
	theMapper.initGovernmentsMapper(input);
	auto match = theMapper.matchGovernment("", "c_test");

	ASSERT_EQ(match->first, "eu4Government2");
}

TEST(Mappers_GovernmentsMapperTests, ck2TitleTakesPriority)
{
	std::stringstream input;
	input << "link = { gov = eu4Government ck2gov = ck2Government }\n";
	input << "link = { gov = eu4Government2 ck2gov = ck2Government2 ck2title = c_test }";

	mappers::GovernmentsMapper theMapper;
	theMapper.initGovernmentsMapper(input);
	auto match = theMapper.matchGovernment("ck2Government", "c_test");

	ASSERT_EQ(match->first, "eu4Government2");
}

TEST(Mappers_GovernmentsMapperTests, reformIsReturnedIfExists)
{
	std::stringstream input;
	input << "link = { gov = eu4Government ck2gov = ck2Government }\n";
	input << "link = { gov = eu4Government2 ck2gov = ck2Government2 ck2title = c_test reform = papacy_reform }";

	mappers::GovernmentsMapper theMapper;
	theMapper.initGovernmentsMapper(input);
	auto match = theMapper.matchGovernment("ck2Government", "c_test");

	ASSERT_EQ(match->first, "eu4Government2");
	ASSERT_EQ(match->second, "papacy_reform");
}

TEST(Mappers_GovernmentsMapperTests, reformIsEmptyIfDoesNotExist)
{
	std::stringstream input;
	input << "link = { gov = eu4Government ck2gov = ck2Government }\n";
	input << "link = { gov = eu4Government2 ck2gov = ck2Government2 ck2title = c_test }";

	mappers::GovernmentsMapper theMapper;
	theMapper.initGovernmentsMapper(input);
	auto match = theMapper.matchGovernment("ck2Government", "c_test");

	ASSERT_TRUE(match->second.empty());
}