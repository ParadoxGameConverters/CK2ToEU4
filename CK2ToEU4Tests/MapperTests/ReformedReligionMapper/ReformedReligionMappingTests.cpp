#include "../../CK2ToEU4/Source/Mappers/ReformedReligionMapper/ReformedReligionMapping.h"
#include "gtest/gtest.h"
#include <sstream>

TEST(Mappers_ReformedReligionMappingTests, doesItGetTheRightIconNumber)
{
	std::stringstream input;
	input << "icon = 1\n}";

	mappers::ReformedReligionMapping theMapper(input);

	const auto& test = theMapper.getIconNumber();
	ASSERT_EQ(1, test);
}

TEST(Mappers_ReformedReligionMappingTests, doesItGetTheRightColors)
{
	std::stringstream input;
	input << "color = { 3 33 333 }\n}";

	mappers::ReformedReligionMapping theMapper(input);

	const auto& test = theMapper.getColor();
	ASSERT_EQ("3 33 333 ", test);
}

TEST(Mappers_ReformedReligionMappingTests, doesItGetTheRightCountryModifiers)
{
	std::stringstream input;
	input << "country = {\ncountry_power = 9001\ncountry_weakness = 0}\n}";

	mappers::ReformedReligionMapping theMapper(input);

	const auto& test = theMapper.getCountryModifiers();
	ASSERT_EQ("country_power =9001\ncountry_weakness =0", test);
}

TEST(Mappers_ReformedReligionMappingTests, doesItGetTheRightProvinceModifiers)
{
	std::stringstream input;
	input << "province = {\nprovince_size = big\nprovince_strength = weak}\n}";

	mappers::ReformedReligionMapping theMapper(input);

	const auto& test = theMapper.getProvinceModifiers();
	ASSERT_EQ("province_size =big\nprovince_strength =weak", test);
}

TEST(Mappers_ReformedReligionMappingTests, doesItGetSecondary)
{
	std::stringstream input;
	input << "country_as_secondary = {\nmingplosion_power = 100\n}\n}";

	mappers::ReformedReligionMapping theMapper(input);

	const auto& test = theMapper.getSecondary();
	ASSERT_EQ("mingplosion_power =100\n", test);
}

TEST(Mappers_ReformedReligionMappingTests, doesItGetUnique)
{
	std::stringstream input;
	input << "unique_modifiers = {\nisUlm = yes\n}\n}";

	mappers::ReformedReligionMapping theMapper(input);

	const auto& test = theMapper.getUniqueMechanics();
	ASSERT_EQ("isUlm =yes\n", test);
}

TEST(Mappers_ReformedReligionMappingTests, doesItGetNonUnique)
{
	std::stringstream input;
	input << "not_unique_modifiers = {\ncan_raid_self = yes\ncan_ignore_treaties = yes\n}\n}";

	mappers::ReformedReligionMapping theMapper(input);

	const auto& test = theMapper.getNonUniqueMechanics();
	ASSERT_EQ("can_raid_self =yes\ncan_ignore_treaties =yes\n", test);
}

TEST(Mappers_ReformedReligionMappingTests, doesItGetHeretics)
{
	std::stringstream input;
	input << "heretic = { WOOD DUCK }\n}";

	mappers::ReformedReligionMapping theMapper(input);

	const auto& test = theMapper.getHereticStrings();
	ASSERT_EQ("WOOD DUCK ", test);
}



