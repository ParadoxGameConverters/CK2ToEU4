#include "../CK2ToEU4/Source/CK2World/Dynasties/Dynasty.h"
#include "gtest/gtest.h"
#include <sstream>

TEST(CK2World_DynastyTests, IDCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	const CK2::Dynasty theDynasty(input, 42);

	ASSERT_EQ(theDynasty.getID(), 42);
}

TEST(CK2World_DynastyTests, cultureCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\tculture=\"paradoxian\"";
	input << "}";

	const CK2::Dynasty theDynasty(input, 42);

	ASSERT_EQ(theDynasty.getCulture(), "paradoxian");
}

TEST(CK2World_DynastyTests, cultureDefaultsToBlank)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	const CK2::Dynasty theDynasty(input, 42);

	ASSERT_TRUE(theDynasty.getCulture().empty());
}

TEST(CK2World_DynastyTests, religionCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\treligion=\"paradoxian\"";
	input << "}";

	const CK2::Dynasty theDynasty(input, 42);

	ASSERT_EQ(theDynasty.getReligion(), "paradoxian");
}

TEST(CK2World_DynastyTests, religionDefaultsToBlank)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	const CK2::Dynasty theDynasty(input, 42);

	ASSERT_TRUE(theDynasty.getReligion().empty());
}

TEST(CK2World_DynastyTests, religionFallsBackOnCoatOfArmsReligion)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "coat_of_arms = { religion = sunni }\n";
	input << "}";

	const CK2::Dynasty theDynasty(input, 42);

	ASSERT_FALSE(theDynasty.getReligion().empty());
	ASSERT_EQ(theDynasty.getReligion(), "sunni");
}

TEST(CK2World_DynastyTests, religionOverridesCoatOfArmsReligion)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "coat_of_arms = { religion = sunni }\n";
	input << "religion = catholic";
	input << "}";

	const CK2::Dynasty theDynasty(input, 42);

	ASSERT_FALSE(theDynasty.getReligion().empty());
	ASSERT_EQ(theDynasty.getReligion(), "catholic");
}

TEST(CK2World_DynastyTests, nameCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\tname=\"paradoxian\"";
	input << "}";

	const CK2::Dynasty theDynasty(input, 42);

	ASSERT_EQ(theDynasty.getName(), "paradoxian");
}

TEST(CK2World_DynastyTests, nameDefaultsToBlank)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	const CK2::Dynasty theDynasty(input, 42);

	ASSERT_TRUE(theDynasty.getName().empty());
}
