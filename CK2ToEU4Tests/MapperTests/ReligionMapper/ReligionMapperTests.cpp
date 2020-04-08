#include "gtest/gtest.h"
#include "../../CK2ToEU4/Source/Mappers/ReligionMapper/ReligionMapper.h"
#include <sstream>

TEST(Mappers_ReligionMapperTests, nonMatchGivesEmptyOptional)
{
	std::stringstream input;
	input << "link = { eu4 = eu4Religion ck2 = ck2Religion }";

	const mappers::ReligionMapper theMapper(input);

	const auto& eu4Religion = theMapper.getEu4ReligionForCk2Religion("nonMatchingReligion");
	ASSERT_FALSE(eu4Religion);
}


TEST(Mappers_ReligionMapperTests, eu4ReligionCanBeFound)
{
	std::stringstream input;
	input << "link = { eu4 = eu4Religion ck2 = ck2Religion }";

	const mappers::ReligionMapper theMapper(input);

	const auto& eu4Religion = theMapper.getEu4ReligionForCk2Religion("ck2Religion");
	ASSERT_EQ(eu4Religion, "eu4Religion");
}


TEST(Mappers_ReligionMapperTests, multipleEU4ReligionsCanBeInARule)
{
	std::stringstream input;
	input << "link = { eu4 = eu4Religion ck2 = ck2Religion ck2 = ck2Religion2 }";

	const mappers::ReligionMapper theMapper(input);

	const auto& eu4Religion = theMapper.getEu4ReligionForCk2Religion("ck2Religion2");
	ASSERT_EQ(eu4Religion, "eu4Religion");
}


TEST(Mappers_ReligionMapperTests, correctRuleMatches)
{
	std::stringstream input;
	input << "link = { eu4 = eu4Religion ck2 = ck2Religion }";
	input << "link = { eu4 = eu4Religion2 ck2 = ck2Religion2 }";

	const mappers::ReligionMapper theMapper(input);

	const auto& eu4Religion = theMapper.getEu4ReligionForCk2Religion("ck2Religion2");
	ASSERT_EQ(eu4Religion, "eu4Religion2");
}