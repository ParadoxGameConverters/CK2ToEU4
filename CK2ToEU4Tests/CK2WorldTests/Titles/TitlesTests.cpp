#include "gtest/gtest.h"
#include <sstream>
#include "../../CK2ToEU4/Source/CK2World/Titles/Titles.h"
#include "../../CK2ToEU4/Source/CK2World/Titles/Title.h"

TEST(CK2World_TitlesTests, titlesDefaultToEmpty)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	const CK2::Titles titles(input);

	ASSERT_TRUE(titles.getTitles().empty());
}

TEST(CK2World_TitlesTests, titleCanBeLoaded)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";	
	input << "c_test={}\n";
	input << "}";

	const CK2::Titles titles(input);

	ASSERT_FALSE(titles.getTitles().empty());
	ASSERT_EQ(titles.getTitles().begin()->first, "c_test");
	ASSERT_EQ(titles.getTitles().begin()->second->getName(), "c_test");
}

TEST(CK2World_TitlesTests, multipleTitlesCanBeLoaded)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "c_test={}\n";
	input << "d_test={}\n";
	input << "e_test={}\n";
	input << "d_dyn_reb_2869601={}\n";
	input << "e_dyn_2653319={}\n";
	input << "}";

	const CK2::Titles titles(input);

	ASSERT_FALSE(titles.getTitles().empty());
	ASSERT_EQ(titles.getTitles().size(), 5);
}
