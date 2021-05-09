#include "../../CK2ToEU4/Source/Mappers/MonumentsMapper/MonumentsMapper.h"
#include "gtest/gtest.h"
#include <sstream>

TEST(Mappers_MonumentsMapperTests, doesItCatchNothing)
{
	std::stringstream input;

	mappers::MonumentsMapper theMapper(input);

	const auto& test = theMapper.getWonders().size();
	ASSERT_EQ(0, test);
}

TEST(Mappers_MonumentsMapperTests, doesItCatchAnything)
{
	std::stringstream input;
	input << "a_wonderful_wonder = {\n}";

	mappers::MonumentsMapper theMapper(input);

	const auto& test = theMapper.getWonders().size();
	ASSERT_EQ(1, test);
}

TEST(Mappers_MonumentsMapperTests, doesItCatchMoreThanOne)
{
	std::stringstream input;
	input << "a_wonderful_wonder = {\n}";
	input << "a_monumental_monument = {\n}";
	input << "Greg = {\n}";

	mappers::MonumentsMapper theMapper(input);

	const auto& test = theMapper.getWonders().size();
	ASSERT_EQ(3, test);
}
