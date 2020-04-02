#include "gtest/gtest.h"
#include "../CK2ToEU4/Source/CK2World/Titles/Title.h"
#include <sstream>

TEST(CK2World_TitleTests, nameCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	const CK2::Title theTitle(input, "c_test");

	ASSERT_EQ(theTitle.getName(), "c_test");
}

TEST(CK2World_TitleTests, lawCanBeAdded)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\tlaw=\"revoke_title_voting_power_0\"\n";	
	input << "}";

	const CK2::Title theTitle(input, "c_test");

	ASSERT_FALSE(theTitle.getLaws().empty());
	ASSERT_EQ(theTitle.getLaws().size(), 1);
	ASSERT_EQ(*theTitle.getLaws().begin(), "revoke_title_voting_power_0");
}

TEST(CK2World_TitleTests, multipleLawsCanBeAdded)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\tlaw=\"revoke_title_voting_power_0\"\n";	
	input << "\tlaw=\"out_of_realm_inheritance_law_1\"\n";	
	input << "\tlaw=\"centralization_4\"\n";	
	input << "}";

	const CK2::Title theTitle(input, "c_test");

	ASSERT_FALSE(theTitle.getLaws().empty());
	ASSERT_EQ(theTitle.getLaws().size(), 3);
	ASSERT_EQ(theTitle.getLaws().count("centralization_4"), 1);
}

TEST(CK2World_TitleTests, holderCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\tholder=123\n";	
	input << "}";

	const CK2::Title theTitle(input, "c_test");

	ASSERT_EQ(theTitle.getHolder(), 123);
}

TEST(CK2World_TitleTests, holderDefaultsToZero)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	const CK2::Title theTitle(input, "c_test");

	ASSERT_EQ(theTitle.getHolder(), 0);
}

TEST(CK2World_TitleTests, liegeTitleDefaultsToBlank)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	const CK2::Title theTitle(input, "c_test");

	ASSERT_TRUE(theTitle.getLiege().getTitle().empty());
}

TEST(CK2World_TitleTests, simpleLiegeCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\tliege=c_test2\n";
	input << "}";

	const CK2::Title theTitle(input, "c_test");

	ASSERT_EQ(theTitle.getLiege().getTitle(), "c_test2");
}

TEST(CK2World_TitleTests, complexLiegeCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\tliege=\n";
	input << "\t{\n";
	input << "\ttitle=c_test2\n";
	input << "\t}\n";
	input << "}";

	const CK2::Title theTitle(input, "c_test");

	ASSERT_EQ(theTitle.getLiege().getTitle(), "c_test2");
}

TEST(CK2World_TitleTests, deJureLiegeTitleDefaultsToBlank)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	const CK2::Title theTitle(input, "c_test");

	ASSERT_TRUE(theTitle.getDeJureLiege().getTitle().empty());
}

TEST(CK2World_TitleTests, simpleDeJureLiegeCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\tde_jure_liege=c_test2\n";
	input << "}";

	const CK2::Title theTitle(input, "c_test");

	ASSERT_EQ(theTitle.getDeJureLiege().getTitle(), "c_test2");
}

TEST(CK2World_TitleTests, complexDeJureLiegeCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\tde_jure_liege=\n";
	input << "\t{\n";
	input << "\ttitle=c_test2\n";
	input << "\t}\n";
	input << "}";

	const CK2::Title theTitle(input, "c_test");

	ASSERT_EQ(theTitle.getDeJureLiege().getTitle(), "c_test2");
}
