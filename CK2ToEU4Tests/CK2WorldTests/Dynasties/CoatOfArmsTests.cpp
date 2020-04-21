#include "../CK2ToEU4/Source/CK2World/Dynasties/CoatOfArms.h"
#include "gtest/gtest.h"
#include <sstream>

TEST(CK2World_CoatOfArmsTests, ReligionDefaultsToBlank)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	const CK2::CoatOfArms coa(input);

	ASSERT_TRUE(coa.getReligion().empty());
}

TEST(CK2World_CoatOfArmsTests, ReligionCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\treligion=\"paradoxian\"";
	input << "}";

	const CK2::CoatOfArms coa(input);

	ASSERT_EQ(coa.getReligion(), "paradoxian");
}
