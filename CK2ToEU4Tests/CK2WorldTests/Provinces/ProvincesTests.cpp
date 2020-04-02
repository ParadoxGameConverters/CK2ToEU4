#include "gtest/gtest.h"
#include <sstream>
#include "../../CK2ToEU4/Source/CK2World/Provinces/Provinces.h"

TEST(CK2World_ProvincesTests, provincesDefaultToEmpty)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	const CK2::Provinces provinces(input);

	ASSERT_TRUE(provinces.getProvinces().empty());
}

TEST(CK2World_ProvincesTests, provinceCanBeLoaded)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "42={}\n";
	input << "}";

	const CK2::Provinces provinces(input);
	const auto& provinceItr = provinces.getProvinces().begin();
	
	ASSERT_EQ(provinceItr->first, 42);
	ASSERT_EQ(provinceItr->second.getID(), 42);
}

