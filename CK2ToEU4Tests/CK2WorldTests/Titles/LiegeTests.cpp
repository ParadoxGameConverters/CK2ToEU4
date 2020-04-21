#include "../CK2ToEU4/Source/CK2World/Titles/Liege.h"
#include "gtest/gtest.h"
#include <sstream>

TEST(CK2World_LiegeTests, titleDefaultsToBlank)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	const CK2::Liege theLiege(input);

	ASSERT_TRUE(theLiege.getTitle().first.empty());
}

TEST(CK2World_LiegeTests, titleCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\ttitle=c_test\n";
	input << "}";

	const CK2::Liege theLiege(input);

	ASSERT_EQ(theLiege.getTitle().first, "c_test");
}

TEST(CK2World_LiegeTests, titleCanBeSetManually)
{
	const CK2::Liege theLiege("c_test");

	ASSERT_EQ(theLiege.getTitle().first, "c_test");
}

TEST(CK2World_LiegeTests, baseTitleDefaultsToBlank)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	const CK2::Liege theLiege(input);

	ASSERT_TRUE(theLiege.getBaseTitle().first.empty());
}

TEST(CK2World_LiegeTests, baseTitleCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\tbase_title=c_test\n";
	input << "}";

	const CK2::Liege theLiege(input);

	ASSERT_EQ(theLiege.getBaseTitle().first, "c_test");
}

TEST(CK2World_LiegeTests, dynamicDefaultsToFalse)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	const CK2::Liege theLiege(input);

	ASSERT_FALSE(theLiege.isDynamic());
}

TEST(CK2World_LiegeTests, dynamicCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\tis_dynamic=yes\n";
	input << "}";

	const CK2::Liege theLiege(input);

	ASSERT_TRUE(theLiege.isDynamic());
}

TEST(CK2World_LiegeTests, customDefaultsToFalse)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	const CK2::Liege theLiege(input);

	ASSERT_FALSE(theLiege.isCustom());
}

TEST(CK2World_LiegeTests, customCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\tis_custom=yes\n";
	input << "}";

	const CK2::Liege theLiege(input);

	ASSERT_TRUE(theLiege.isCustom());
}
