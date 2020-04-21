#include "../CK2ToEU4/Source/Mappers/ProvinceMapper/ProvinceMapper.h"
#include "gtest/gtest.h"
#include <sstream>

TEST(Mappers_ProvinceMapperTests, emptyMappingsDefaultToEmpty)
{
	std::stringstream input;
	input << "0.0.0.0 = {\n";
	input << "}";

	const mappers::ProvinceMapper theMapper(input);

	ASSERT_TRUE(theMapper.getCK2ProvinceNumbers(1).empty());
}

TEST(Mappers_ProvinceMapperTests, canLookupCK2Provinces)
{
	std::stringstream input;
	input << "0.0.0.0 = {\n";
	input << "	link = { eu4 = 2 eu4 = 1 ck2 = 2 ck2 = 1 }\n";
	input << "}";

	const mappers::ProvinceMapper theMapper(input);

	ASSERT_EQ(theMapper.getCK2ProvinceNumbers(1).size(), 2);
	ASSERT_EQ(theMapper.getCK2ProvinceNumbers(1)[0], 2);
	ASSERT_EQ(theMapper.getCK2ProvinceNumbers(1)[1], 1);
	ASSERT_EQ(theMapper.getCK2ProvinceNumbers(2).size(), 2);
	ASSERT_EQ(theMapper.getCK2ProvinceNumbers(2)[0], 2);
	ASSERT_EQ(theMapper.getCK2ProvinceNumbers(2)[1], 1);
}

TEST(Mappers_ProvinceMapperTests, canLookupEU4Provinces)
{
	std::stringstream input;
	input << "0.0.0.0 = {\n";
	input << "	link = { eu4 = 2 eu4 = 1 ck2 = 2 ck2 = 1 }\n";
	input << "}";

	const mappers::ProvinceMapper theMapper(input);

	ASSERT_EQ(theMapper.getEU4ProvinceNumbers(1).size(), 2);
	ASSERT_EQ(theMapper.getEU4ProvinceNumbers(1)[0], 2);
	ASSERT_EQ(theMapper.getEU4ProvinceNumbers(1)[1], 1);
	ASSERT_EQ(theMapper.getEU4ProvinceNumbers(2).size(), 2);
	ASSERT_EQ(theMapper.getEU4ProvinceNumbers(2)[0], 2);
	ASSERT_EQ(theMapper.getEU4ProvinceNumbers(2)[1], 1);
}

TEST(Mappers_ProvinceMapperTests, chineseProvincesDefaultToEmpty)
{
	std::stringstream dummyInput;
	const mappers::ProvinceMapper theMapper(dummyInput);

	ASSERT_TRUE(theMapper.getOffmapChineseProvinces().empty());
}

TEST(Mappers_ProvinceMapperTests, chineseProvincesCanBeLoaded)
{
	std::stringstream dummyInput;
	mappers::ProvinceMapper theMapper(dummyInput);
	std::stringstream input;
	input << "chinese_offmap_provinces = { 1816 1821 1822 }";
	theMapper.loadOffmapChineseProvinces(input);

	ASSERT_EQ(theMapper.getOffmapChineseProvinces().size(), 3);
	ASSERT_EQ(*theMapper.getOffmapChineseProvinces().find(1816), 1816);
	ASSERT_EQ(*theMapper.getOffmapChineseProvinces().find(1821), 1821);
	ASSERT_EQ(*theMapper.getOffmapChineseProvinces().find(1822), 1822);
}
