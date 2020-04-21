#include "../CK2ToEU4/Source/Mappers/ProvinceMapper/ProvinceMappingsVersion.h"
#include "gtest/gtest.h"
#include <sstream>
#include <string>

TEST(Mappers_ProvinceMappingsVersionTests, MappingsDefaultToEmpty)
{
	std::stringstream input;
	input << "= {\n";
	input << "}";

	const mappers::ProvinceMappingsVersion theMappingVersion(input);

	ASSERT_TRUE(theMappingVersion.getMappings().empty());
}

TEST(Mappers_ProvinceMappingsVersionTests, MappingsCanBeLoaded)
{
	std::stringstream input;
	input << "= {\n";
	input << "	link = { eu4 = 1 ck2 = 1 }\n";
	input << "	link = { eu4 = 2 ck2 = 2 }\n";
	input << "}";

	const mappers::ProvinceMappingsVersion theMappingVersion(input);

	ASSERT_EQ(theMappingVersion.getMappings().size(), 2);
}