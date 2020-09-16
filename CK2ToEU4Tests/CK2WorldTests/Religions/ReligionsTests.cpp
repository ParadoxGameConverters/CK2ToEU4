#include "../../CK2ToEU4/Source/CK2World/Religions/Religions.h"
#include "gtest/gtest.h"
#include <sstream>

TEST(CK2World_ReligionsTests, doesItCatchNoReligions)
{
	std::stringstream input;

	CK2::Religions theMapper(input);

	auto test = theMapper.getReformedReligion();

	std::map<std::string, std::vector<std::string>> tester;

	ASSERT_EQ(tester, test);
}

TEST(CK2World_ReligionsTests, doesItCatchReligionName)
{
	std::stringstream input;
	input << "test_reformed={life is meaningless}";

	CK2::Religions theMapper(input);

	const auto& test = theMapper.getReformedReligion().find("test_reformed")->first;
	ASSERT_EQ("test_reformed", test);
}

TEST(CK2World_ReligionsTests, doesItCatchReligionsName)
{
	std::stringstream input;
	input << "test_reformed={}\n";
	input << "test_atheism={}\n";
	input << "test_illuminati={}";

	CK2::Religions theMapper(input);

	const auto test = theMapper.getReformedReligion();

	ASSERT_EQ(3, test.size());
}