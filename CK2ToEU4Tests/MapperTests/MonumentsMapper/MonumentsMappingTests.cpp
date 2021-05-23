#include "../../CK2ToEU4/Source/Mappers/MonumentsMapper/MonumentsMapping.h"
#include "gtest/gtest.h"
#include <sstream>
#include <string>
#include <iostream>
using namespace std;

// Default
TEST(Mappers_MonumentsMapping, defaultClassTest)
{
	mappers::MonumentsMapping theMapper;

	const auto& test = theMapper.getCanBeMoved();
	ASSERT_EQ(false, test);
}

// Getters
TEST(Mappers_MonumentsMappingTests, doesItGetBase)
{
	std::stringstream input;
	input << "properties = {\n";
	input << "\tcan_be_moved = yes\n";
	input << "}";

	mappers::MonumentsMapping theMapper(input);

	const auto& test = theMapper.getIsBase();
	ASSERT_EQ(1, test);
}

TEST(Mappers_MonumentsMappingTests, doesItGetTheRightProvinceModifiers)
{
	std::stringstream input;
	input << "province_modifiers = { local_defensiveness = { 0.05 0.2 0.33 0.33 } }";

	mappers::MonumentsMapping theMapper(input);

	const auto& test = theMapper.getProvinceModifiers();

	std::string a = "local_defensiveness";
	std::vector<double> b = {0.05, 0.2, 0.33, 0.33};

	ASSERT_EQ(test.find(a)->first, a);
	ASSERT_EQ(test.find(a)->second, b);	
}

TEST(Mappers_MonumentsMappingTests, doesItGetTheRightAreaModifiers)
{
	std::stringstream input;
	input << "area_modifiers = { local_defensiveness = { 0.05 0.2 0.33 0.33 } }";

	mappers::MonumentsMapping theMapper(input);

	const auto& test = theMapper.getAreaModifiers();

	std::string a = "local_defensiveness";
	std::vector<double> b = {0.05, 0.2, 0.33, 0.33};

	ASSERT_EQ(test.find(a)->first, a);
	ASSERT_EQ(test.find(a)->second, b);
}

TEST(Mappers_MonumentsMappingTests, doesItGetTheRightCountryModifiers)
{
	std::stringstream input;
	input << "country_modifiers = { defensiveness = { 0.05 0.2 0.33 0.33 } }";

	mappers::MonumentsMapping theMapper(input);

	const auto& test = theMapper.getCountryModifiers();

	std::string a = "defensiveness";
	std::vector<double> b = {0.05, 0.2, 0.33, 0.33};

	ASSERT_EQ(test.find(a)->first, a);
	ASSERT_EQ(test.find(a)->second, b);
}

TEST(Mappers_MonumentsMappingTests, doesItGetTheRightOnUpgraded)
{
	std::stringstream input;
	input << "on_upgraded = { do a thing = yes }";

	mappers::MonumentsMapping theMapper(input);

	const auto& test = theMapper.getOnUpgraded();

	ASSERT_EQ(" do a thing = yes ", test);
}

TEST(Mappers_MonumentsMappingTests, doesItGetBuilderCulture)
{
	std::stringstream input;
	input << "build_trigger = { cultural }";

	mappers::MonumentsMapping theMapper(input);

	const auto& test = theMapper.isOfBuilderCulture();

	ASSERT_EQ(1, test);
}

TEST(Mappers_MonumentsMappingTests, doesItGetBuilderReligion)
{
	std::stringstream input;
	input << "build_trigger = { religious }";

	mappers::MonumentsMapping theMapper(input);

	const auto& test = theMapper.isOfBuilderReligion();

	ASSERT_EQ(1, test);
}

// Setters
TEST(Mappers_MonumentsMappingTests, doesItSetCanBeMoved)
{
	std::stringstream input;
	mappers::MonumentsMapping theMapper(input);
	theMapper.setCanBeMoved(true);

	const auto& test = theMapper.getCanBeMoved();
	ASSERT_EQ(1, test);
}
