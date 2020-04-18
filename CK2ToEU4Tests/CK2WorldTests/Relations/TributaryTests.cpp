#include "../CK2ToEU4/Source/CK2World/Relations/Tributary.h"
#include "gtest/gtest.h"
#include <sstream>

TEST(CK2World_TributaryTests, TributaryIDDefaultsToZero)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	const CK2::Tributary tributary(input);

	ASSERT_FALSE(tributary.getTributaryID());
}

TEST(CK2World_TributaryTests, TributaryIDCanBeLoaded)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "tributary = 7\n";
	input << "}";

	const CK2::Tributary tributary(input);

	ASSERT_EQ(tributary.getTributaryID(), 7);
}

TEST(CK2World_TributaryTests, TributaryTypeDefaultsToBlank)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	const CK2::Tributary tributary(input);

	ASSERT_TRUE(tributary.getTributaryType().empty());
}

TEST(CK2World_TributaryTests, TributaryTypeCanBeLoaded)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "tributary_type = default\n";
	input << "}";

	const CK2::Tributary tributary(input);

	ASSERT_EQ(tributary.getTributaryType(), "default");
}
