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