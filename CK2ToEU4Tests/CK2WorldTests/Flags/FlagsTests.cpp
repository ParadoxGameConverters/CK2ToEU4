#include "../../CK2ToEU4/Source/CK2World/Flags/Flags.h"
#include "gtest/gtest.h"
#include <sstream>

TEST(CK2World_FlagsTests, doesItCatchFlags)
{
	std::stringstream input;
	input << "worldIsReal=yes\n";
	input << "worldGovernment=yes\n";
	input << "worldReligion=no";

	CK2::Flags theMapper(input);

	std::set<std::string> tester = {"worldIsReal", "worldGovernment", "worldReligion"};
	const auto& test = theMapper.getFlags();

	ASSERT_EQ(tester, test);
}

TEST(CK2World_FlagsTests, doesItCatchNothing)
{
	std::stringstream input;

	CK2::Flags theMapper(input);

	const auto& test = theMapper.getFlags();

	std::set<std::string> tester;

	ASSERT_EQ(tester, test);
}

TEST(CK2World_FlagsTests, doesItFillNothing)
{
	std::stringstream input;

	CK2::Flags theMapper(input);

	const auto& test = theMapper.fillReformationList();

	std::set<std::string> tester;

	ASSERT_EQ(tester, test);
}

TEST(CK2World_FlagsTests, doesItFillThings)
{
	std::stringstream input;
	input << "zun_reformation=769.1.12\n";
	input << "bon_reformation = 769.1.19\n";
	input << "west_african_reformation = 769.1.23";

	CK2::Flags theMapper(input);

	const auto& test = theMapper.fillReformationList();

	std::set<std::string> tester = { "zun_pagan_reformed", "bon_reformed", "west_african_pagan_reformed" };

	ASSERT_EQ(tester, test);
}