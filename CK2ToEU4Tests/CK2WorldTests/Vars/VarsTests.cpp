#include "../../CK2ToEU4/Source/CK2World/Vars/Vars.h"
#include "gtest/gtest.h"
#include <sstream>

TEST(CK2World_VarsTests, varsDefaultToEmpty)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	const CK2::Vars vars(input);

	ASSERT_TRUE(vars.getVars().empty());
}

TEST(CK2World_VarsTests, varsCanBeLoaded)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "a_variaBLE1 = 8.900\n";
	input << "SECOND-variable_2 = 0.000\n";
	input << "}";

	const CK2::Vars vars(input);
	const auto& firstVariable = vars.getVars().find("a_variaBLE1");
	const auto& secondVariable = vars.getVars().find("SECOND-variable_2");

	ASSERT_FALSE(vars.getVars().empty());
	ASSERT_NEAR(firstVariable->second, 8.9, 0.001);
	ASSERT_EQ(secondVariable->second, 0);
}

TEST(CK2World_VarsTests, chineseReligionsReturnsNullForNoReligions)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "a_variaBLE1 = 8.900\n";
	input << "SECOND-variable_2 = 0.000\n";
	input << "}";

	const CK2::Vars vars(input);

	ASSERT_FALSE(vars.getChineseReligions());
}

TEST(CK2World_VarsTests, chineseReligionsReturnsChineseReligions)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "a_variaBLE1 = 8.900\n";
	input << "SECOND-variable_2 = 0.000\n";
	input << "global_chinese_christians = 20.000\n";
	input << "global_chinese_sunnis = 40.000\n";
	input << "}";

	const CK2::Vars vars(input);
	const auto& chineseReligions = vars.getChineseReligions();
	const auto christian = chineseReligions->find("christian");
	const auto sunni = chineseReligions->find("sunni");

	ASSERT_FALSE(chineseReligions->empty());
	ASSERT_EQ(chineseReligions->size(), 2);
	ASSERT_NEAR(christian->second, 20, 0.001);
	ASSERT_NEAR(sunni->second, 40, 0.001);
}
