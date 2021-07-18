#include "../../CK2ToEU4/Source/Mappers/ReformedReligionMapper/ReformedReligionMapping.h"
#include "gtest/gtest.h"
#include <sstream>

// Default
TEST(Mappers_ReformedReligionMappingTests, defaultClassTest)
{
	mappers::ReformedReligionMapping theMapper;

	const auto& test = theMapper.getIconNumber();
	ASSERT_EQ(0, test);
}

// Getters
TEST(Mappers_ReformedReligionMappingTests, doesItGetTheRightIconNumber)
{
	std::stringstream input;
	input << "= {\n";
	input << "\ticon = 1\n";
	input << "}";

	mappers::ReformedReligionMapping theMapper(input);

	const auto& test = theMapper.getIconNumber();
	ASSERT_EQ(1, test);
}

TEST(Mappers_ReformedReligionMappingTests, doesItGetTheRightCountryModifiers)
{
	std::stringstream input;
	input << "country = {\ncountry_power = 9001\n";
	input << "country_weakness = 0}\n}";

	mappers::ReformedReligionMapping theMapper(input);

	const auto& test = theMapper.getCountryModifiers();
	ASSERT_EQ("\ncountry_power = 9001\ncountry_weakness = 0", test);
}

TEST(Mappers_ReformedReligionMappingTests, doesItGetTheRightProvinceModifiers)
{
	std::stringstream input;
	input << "province = {\nprovince_size = big\n";
	input << "province_strength = weak}\n}";

	mappers::ReformedReligionMapping theMapper(input);

	const auto& test = theMapper.getProvinceModifiers();
	ASSERT_EQ("\nprovince_size = big\nprovince_strength = weak", test);
}

TEST(Mappers_ReformedReligionMappingTests, doesItGetSecondary)
{
	std::stringstream input;
	input << "country_as_secondary = {\n";
	input << "mingplosion_power = 100\n}\n}";

	mappers::ReformedReligionMapping theMapper(input);

	const auto& test = theMapper.getSecondary();
	ASSERT_EQ("\nmingplosion_power = 100\n", test);
}

TEST(Mappers_ReformedReligionMappingTests, doesItGetUnique)
{
	std::stringstream input;
	input << "unique_modifiers = {\n";
	input << "isUlm = yes\n}\n}";

	mappers::ReformedReligionMapping theMapper(input);

	const auto& test = theMapper.getUniqueMechanics();
	ASSERT_EQ("\nisUlm = yes\n", test);
}

TEST(Mappers_ReformedReligionMappingTests, doesItGetNonUnique)
{
	std::stringstream input;
	input << "not_unique_modifiers = {\n";
	input << "can_raid_self = yes\n";
	input << "can_ignore_treaties = yes\n}\n}";

	mappers::ReformedReligionMapping theMapper(input);

	const auto& test = theMapper.getNonUniqueMechanics();
	ASSERT_EQ("\ncan_raid_self = yes\ncan_ignore_treaties = yes\n", test);
}

TEST(Mappers_ReformedReligionMappingTests, doesItGetHeretics)
{
	std::stringstream input;
	input << "heretic = { WOOD DUCK }\n}";

	mappers::ReformedReligionMapping theMapper(input);

	const auto& test = theMapper.getHereticStrings();
	ASSERT_EQ(" WOOD DUCK ", test);
}

// Setters
TEST(Mappers_ReformedReligionMappingTests, doesItSetTheRightIconNumber)
{
	std::stringstream input;
	mappers::ReformedReligionMapping theMapper(input);
	theMapper.setIconNumber(1);

	const auto& test = theMapper.getIconNumber();
	ASSERT_EQ(1, test);
}

TEST(Mappers_ReformedReligionMappingTests, doesItSetTheRightCountryModifiers)
{
	std::stringstream input;
	mappers::ReformedReligionMapping theMapper(input);
	theMapper.setCountryModifiers("country_power =9001\ncountry_weakness =0");
	const auto& test = theMapper.getCountryModifiers();

	ASSERT_EQ("country_power =9001\ncountry_weakness =0", test);
}

TEST(Mappers_ReformedReligionMappingTests, doesItSetTheRightProvinceModifiers)
{
	std::stringstream input;
	mappers::ReformedReligionMapping theMapper(input);
	theMapper.setProvinceModifiers("province_size =big\nprovince_strength =weak");
	const auto& test = theMapper.getProvinceModifiers();
	ASSERT_EQ("province_size =big\nprovince_strength =weak", test);
}

TEST(Mappers_ReformedReligionMappingTests, doesItSetSecondary)
{
	std::stringstream input;
	mappers::ReformedReligionMapping theMapper(input);
	theMapper.setSecondary("mingplosion_power =100\n");

	const auto& test = theMapper.getSecondary();
	ASSERT_EQ("mingplosion_power =100\n", test);
}

TEST(Mappers_ReformedReligionMappingTests, doesItSetUnique)
{
	std::stringstream input;
	mappers::ReformedReligionMapping theMapper(input);
	theMapper.setUniqueMechanics("isUlm =yes\n");

	const auto& test = theMapper.getUniqueMechanics();
	ASSERT_EQ("isUlm =yes\n", test);
}

TEST(Mappers_ReformedReligionMappingTests, doesItSetNonUnique)
{
	std::stringstream input;
	mappers::ReformedReligionMapping theMapper(input);
	theMapper.setNonUniqueMechanics("can_raid_self =yes\ncan_ignore_treaties =yes\n");

	const auto& test = theMapper.getNonUniqueMechanics();
	ASSERT_EQ("can_raid_self =yes\ncan_ignore_treaties =yes\n", test);
}

TEST(Mappers_ReformedReligionMappingTests, doesItSetHeretics)
{
	std::stringstream input;
	mappers::ReformedReligionMapping theMapper(input);
	theMapper.setHereticStrings("WOOD DUCK ");

	const auto& test = theMapper.getHereticStrings();
	ASSERT_EQ("WOOD DUCK ", test);
}
