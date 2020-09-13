#include "../../CK2ToEU4/Source/CK2World/Religions/Religion.h"
#include "gtest/gtest.h"
#include <sstream>

TEST(CK2World_ReligionTests, doesItCatchFeatures)
{
	std::stringstream input;
	input << "features = {\n";
	input << "feature_1 feature_2 feature_3\n";
	input << "}";

	CK2::Religion theMapper(input);

	std::vector<std::string> tester = {"feature_1", "feature_2", "feature_3"};
	const auto& test = theMapper.getFeatures();

	ASSERT_EQ(tester, test);
}

TEST(CK2World_ReligionTests, doesItCatchNoFeatures)
{
	std::stringstream input;

	CK2::Religion theMapper(input);
	const auto& test = theMapper.getFeatures();

	std::vector<std::string> tester;

	ASSERT_EQ(tester, test);
}