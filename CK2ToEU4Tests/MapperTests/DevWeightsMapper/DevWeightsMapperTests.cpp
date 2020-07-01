#include "../CK2ToEU4/Source/Mappers/DevWeightsMapper/DevWeightsMapper.h"
#include "gtest/gtest.h"
#include <sstream>

TEST(Mappers_DevWeightsMapperTests, devFromBuildingDefaultsToDefault)
{
	std::stringstream input;

	const mappers::DevWeightsMapper theMapper(input);

	ASSERT_NEAR(theMapper.getDevFromBuilding(), 0.1, 0.001);
}

TEST(Mappers_DevWeightsMapperTests, devFromBuildingCanBeSet)
{
	std::stringstream input;
	input << "dev_from_building = 0.2\n";

	const mappers::DevWeightsMapper theMapper(input);

	ASSERT_NEAR(theMapper.getDevFromBuilding(), 0.2, 0.001);
}

TEST(Mappers_DevWeightsMapperTests, devFromBaronyDefaultsToDefault)
{
	std::stringstream input;

	const mappers::DevWeightsMapper theMapper(input);

	ASSERT_NEAR(theMapper.getDevFromBarony(), 0.3, 0.001);
}

TEST(Mappers_DevWeightsMapperTests, devFromBaronyCanBeSet)
{
	std::stringstream input;
	input << "dev_from_barony = 0.5\n";

	const mappers::DevWeightsMapper theMapper(input);

	ASSERT_NEAR(theMapper.getDevFromBarony(), 0.5, 0.001);
}
