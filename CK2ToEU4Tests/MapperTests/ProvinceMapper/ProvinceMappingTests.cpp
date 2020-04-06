#include "gtest/gtest.h"
#include "../CK2ToEU4/Source/Mappers/ProvinceMapper/ProvinceMapping.h"
#include <sstream>

TEST(Mappers_ProvinceMappingTests, EU4ProvinceCanBeAdded)
{
	std::stringstream input;
	input << "= { eu4 = 2 eu4 = 1 }";

	const mappers::ProvinceMapping theMapper(input);

	ASSERT_EQ(theMapper.getEU4Provinces()[0], 2);
	ASSERT_EQ(theMapper.getEU4Provinces()[1], 1);
}

TEST(Mappers_ProvinceMappingTests, CK2ProvinceCanBeAdded)
{
	std::stringstream input;
	input << "= { ck2 = 2 ck2 = 1 }";

	const mappers::ProvinceMapping theMapper(input);

	ASSERT_EQ(theMapper.getCK2Provinces()[0], 2);
	ASSERT_EQ(theMapper.getCK2Provinces()[1], 1);
}
