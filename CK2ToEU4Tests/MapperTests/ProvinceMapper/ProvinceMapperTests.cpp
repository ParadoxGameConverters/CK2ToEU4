#include "gtest/gtest.h"
#include "../CK2ToEU4/Source/Mappers/ProvinceMapper/ProvinceMapper.h"
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