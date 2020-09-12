#include "../../CK2ToEU4/Source/Mappers/ReformedReligionMapper/ReformedReligionMapper.h"
#include "gtest/gtest.h"
#include <sstream>

TEST(Mappers_ReformedReligionMapperTests, doesItCatchAnything)
{
	std::stringstream input;
	input << "a_religion_reformed = {\n}";

	mappers::ReformedReligionMapper theMapper(input);

	const auto& test = theMapper.getReligionEntries().size();
	ASSERT_EQ(1, test);
}

TEST(Mappers_ReformedReligionMapperTests, doesItCatchMoreThanOne)
{
	std::stringstream input;
	input << "a_religion_reformed = {\n}" << "a_new_religion_reformed = {\n}" << "a_newer_religion_reformed = {\n}";

	mappers::ReformedReligionMapper theMapper(input);

	const auto& test = theMapper.getReligionEntries().size();
	ASSERT_EQ(3, test);
}

TEST(Mappers_ReformedReligionMapperTests, doesItIgnoreComments)
{
	std::stringstream input;
	input << "#BLAH";

	mappers::ReformedReligionMapper theMapper(input);

	const auto& test = theMapper.getReligionEntries().size();
	ASSERT_EQ(0, test);
}

TEST(Mappers_ReformedReligionMapperTests, doesItGetTheRightName)
{
	std::stringstream input;
	input << "a_religion_reformed = {\n}";

	mappers::ReformedReligionMapper theMapper(input);

	const auto& test = theMapper.getReligionEntries().find("a_religion_reformed")->second.getName();
	ASSERT_EQ("a_religion_reformed", test);
}
