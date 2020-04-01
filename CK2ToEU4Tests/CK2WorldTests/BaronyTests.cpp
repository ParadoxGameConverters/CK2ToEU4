#include "gtest/gtest.h"
#include "../CK2ToEU4/Source/CK2World/Provinces/Province.h"
#include <sstream>

TEST(CK2World_BaronyTests, nameCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	CK2::Barony theBarony(input, "b_test");

	ASSERT_EQ(theBarony.getName(), "b_test");
}

TEST(CK2World_BaronyTests, typeCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\ttype=\"castle\"";
	input << "}";

	CK2::Barony theBarony(input, "b_test");

	ASSERT_EQ(theBarony.getType(), "castle");
}

TEST(CK2World_BaronyTests, typeDefaultsToBlank)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	CK2::Barony theBarony(input, "b_test");

	ASSERT_EQ(theBarony.getType(), "");
}

TEST(CK2World_BaronyTests, castleBuildingsAreRecognized)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "ca_stable_3=yes\n";
	input << "ca_town_1=yes\n";
	input << "}";
	CK2::Barony theBarony(input, "b_test");

	ASSERT_EQ(theBarony.getBuildingCount(), 2);
}

TEST(CK2World_BaronyTests, templeBuildingsAreRecognized)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "tp_monastery_3=yes\n";
	input << "tp_school_1=yes\n";
	input << "}";
	CK2::Barony theBarony(input, "b_test");

	ASSERT_EQ(theBarony.getBuildingCount(), 2);
}

TEST(CK2World_BaronyTests, townBuildingsAreRecognized)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "ct_port_1=yes\n";
	input << "ct_barracks_3=yes\n";
	input << "}";
	CK2::Barony theBarony(input, "b_test");

	ASSERT_EQ(theBarony.getBuildingCount(), 2);
}

TEST(CK2World_BaronyTests, mixedBuildingsAreRecognized)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "ca_shipyard_1=yes\n";
	input << "ct_barracks_3=yes\n";
	input << "tp_barracks_1=yes\n";
	input << "}";
	CK2::Barony theBarony(input, "b_test");

	ASSERT_EQ(theBarony.getBuildingCount(), 3);
}
