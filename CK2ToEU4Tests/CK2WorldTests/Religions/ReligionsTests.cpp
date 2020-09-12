#include "../../CK2ToEU4/Source/CK2World/Religions/Religions.h"
#include "gtest/gtest.h"
#include <sstream>

TEST(CK2World_ReligionsTests, doesItCatchReligionName)
{
	std::stringstream input;
	input << "test_reformed={life is meaningless}";

	CK2::Religions theMapper(input);

	const auto& test = theMapper.getReformedReligion().find("test_reformed")->first;
	ASSERT_EQ("test_reformed", test);
}