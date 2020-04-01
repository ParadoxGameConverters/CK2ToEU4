#include "gtest/gtest.h"
#include "../CK2ToEU4/Source/CK2World/Provinces/Province.h"
#include <sstream>



TEST(CK2World_ProvinceTests, numberCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	CK2::Province theProvince(input, "42");

	ASSERT_EQ(theProvince.getNumber(), 42);
}


TEST(CK2World_ProvinceTests, ownerDefaultsToBlank)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	Vic2::Province theProvince("42", input);

	ASSERT_EQ(theProvince.getOwner(), "");
}


TEST(CK2World_ProvinceTests, ownerCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\towner=\"TAG\"";
	input << "}";

	Vic2::Province theProvince("42", input);

	ASSERT_EQ(theProvince.getOwner(), "TAG");
}


TEST(CK2World_ProvinceTests, controllerDefaultsToBlank)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	Vic2::Province theProvince("42", input);

	ASSERT_EQ(theProvince.getController(), "");
}


TEST(CK2World_ProvinceTests, controllerCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\tcontroller=\"TAG\"";
	input << "}";

	Vic2::Province theProvince("42", input);

	ASSERT_EQ(theProvince.getController(), "TAG");
}


TEST(CK2World_ProvinceTests, ownerCanBeChanged)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\towner=\"TAG\"";
	input << "}";

	Vic2::Province theProvince("42", input);
	theProvince.setOwner("NEW");

	ASSERT_EQ(theProvince.getOwner(), "NEW");
}


TEST(CK2World_ProvinceTests, coresDefaultToEmpty)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	Vic2::Province theProvince("42", input);

	ASSERT_EQ(theProvince.getCores().size(), 0);
}


TEST(CK2World_ProvinceTests, coresCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\tcore=\"TAG\"";
	input << "\tcore=\"2ND\"";
	input << "}";

	Vic2::Province theProvince("42", input);

	ASSERT_EQ(theProvince.getCores().size(), 2);
	ASSERT_EQ(theProvince.getCores().count("TAG"), 1);
	ASSERT_EQ(theProvince.getCores().count("2ND"), 1);
}


TEST(CK2World_ProvinceTests, coresCanBeRemoved)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\tcore=\"TAG\"";
	input << "\tcore=\"2ND\"";
	input << "}";

	Vic2::Province theProvince("42", input);
	theProvince.removeCore("2ND");

	ASSERT_EQ(theProvince.getCores().size(), 1);
	ASSERT_EQ(theProvince.getCores().count("TAG"), 1);
}


TEST(CK2World_ProvinceTests, popsDefaultToEmpty)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	Vic2::Province theProvince("42", input);

	ASSERT_EQ(theProvince.getPops().size(), 0);
}


TEST(CK2World_ProvinceTests, aristocratsCanBeAddedToPops)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\taristocrats={\n";
	input << "\t}\n";
	input << "}";

	Vic2::Province theProvince("42", input);

	ASSERT_EQ(theProvince.getPops().size(), 1);
	ASSERT_EQ(theProvince.getPops()[0].getType(), "aristocrats");
}


TEST(CK2World_ProvinceTests, artisansCanBeAddedToPops)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\tartisans={\n";
	input << "\t}\n";
	input << "}";

	Vic2::Province theProvince("42", input);

	ASSERT_EQ(theProvince.getPops().size(), 1);
	ASSERT_EQ(theProvince.getPops()[0].getType(), "artisans");
}


TEST(CK2World_ProvinceTests, bureaucratsCanBeAddedToPops)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\tbureaucrats={\n";
	input << "\t}\n";
	input << "}";

	Vic2::Province theProvince("42", input);

	ASSERT_EQ(theProvince.getPops().size(), 1);
	ASSERT_EQ(theProvince.getPops()[0].getType(), "bureaucrats");
}


TEST(CK2World_ProvinceTests, capitalistsCanBeAddedToPops)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\tcapitalists={\n";
	input << "\t}\n";
	input << "}";

	Vic2::Province theProvince("42", input);

	ASSERT_EQ(theProvince.getPops().size(), 1);
	ASSERT_EQ(theProvince.getPops()[0].getType(), "capitalists");
}


TEST(CK2World_ProvinceTests, clergymenCanBeAddedToPops)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\tclergymen={\n";
	input << "\t}\n";
	input << "}";

	Vic2::Province theProvince("42", input);

	ASSERT_EQ(theProvince.getPops().size(), 1);
	ASSERT_EQ(theProvince.getPops()[0].getType(), "clergymen");
}


TEST(CK2World_ProvinceTests, craftsmenCanBeAddedToPops)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\tcraftsmen={\n";
	input << "\t}\n";
	input << "}";

	Vic2::Province theProvince("42", input);

	ASSERT_EQ(theProvince.getPops().size(), 1);
	ASSERT_EQ(theProvince.getPops()[0].getType(), "craftsmen");
}


TEST(CK2World_ProvinceTests, clerksCanBeAddedToPops)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\tclerks={\n";
	input << "\t}\n";
	input << "}";

	Vic2::Province theProvince("42", input);

	ASSERT_EQ(theProvince.getPops().size(), 1);
	ASSERT_EQ(theProvince.getPops()[0].getType(), "clerks");
}


TEST(CK2World_ProvinceTests, farmersCanBeAddedToPops)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\tfarmers={\n";
	input << "\t}\n";
	input << "}";

	Vic2::Province theProvince("42", input);

	ASSERT_EQ(theProvince.getPops().size(), 1);
	ASSERT_EQ(theProvince.getPops()[0].getType(), "farmers");
}


TEST(CK2World_ProvinceTests, soldiersCanBeAddedToPops)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\tsoldiers={\n";
	input << "\t}\n";
	input << "}";

	Vic2::Province theProvince("42", input);

	ASSERT_EQ(theProvince.getPops().size(), 1);
	ASSERT_EQ(theProvince.getPops()[0].getType(), "soldiers");
}


TEST(CK2World_ProvinceTests, officersCanBeAddedToPops)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\tofficers={\n";
	input << "\t}\n";
	input << "}";

	Vic2::Province theProvince("42", input);

	ASSERT_EQ(theProvince.getPops().size(), 1);
	ASSERT_EQ(theProvince.getPops()[0].getType(), "officers");
}


TEST(CK2World_ProvinceTests, labourersCanBeAddedToPops)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\tlabourers={\n";
	input << "\t}\n";
	input << "}";

	Vic2::Province theProvince("42", input);

	ASSERT_EQ(theProvince.getPops().size(), 1);
	ASSERT_EQ(theProvince.getPops()[0].getType(), "labourers");
}


TEST(CK2World_ProvinceTests, slavesCanBeAddedToPops)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\tslaves={\n";
	input << "\t}\n";
	input << "}";

	Vic2::Province theProvince("42", input);

	ASSERT_EQ(theProvince.getPops().size(), 1);
	ASSERT_EQ(theProvince.getPops()[0].getType(), "slaves");
}


TEST(CK2World_ProvinceTests, serfsCanBeAddedToPops)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\tserfs={\n";
	input << "\t}\n";
	input << "}";

	Vic2::Province theProvince("42", input);

	ASSERT_EQ(theProvince.getPops().size(), 1);
	ASSERT_EQ(theProvince.getPops()[0].getType(), "serfs");
}


TEST(CK2World_ProvinceTests, getTotalPopulationDefaultsToZero)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	Vic2::Province theProvince("42", input);

	ASSERT_EQ(theProvince.getTotalPopulation(), 0);
}


TEST(CK2World_ProvinceTests, getTotalPopulationReturnsTotalPopulation)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\tserfs={\n";
	input << "\t\tsize=1";
	input << "\t}\n";
	input << "\tslaves={\n";
	input << "\t\tsize=2";
	input << "\t}\n";
	input << "}";

	Vic2::Province theProvince("42", input);

	ASSERT_EQ(theProvince.getTotalPopulation(), 3);
}


TEST(CK2World_ProvinceTests, getPopulationDefaultsToZero)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	Vic2::Province theProvince("42", input);

	ASSERT_EQ(theProvince.getPopulation(), 0);
}


TEST(CK2World_ProvinceTests, getPopulationDiscriminatesBySpecifiedPopType)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\tserfs={\n";
	input << "\t\tsize=1";
	input << "\t}\n";
	input << "\tslaves={\n";
	input << "\t\tsize=2";
	input << "\t}\n";
	input << "}";

	Vic2::Province theProvince("42", input);

	ASSERT_EQ(theProvince.getPopulation("slaves"), 2);
}


TEST(CK2World_ProvinceTests, getPopulationWithNoTypeGivesTotalPopulation)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\tserfs={\n";
	input << "\t\tsize=1";
	input << "\t}\n";
	input << "\tslaves={\n";
	input << "\t\tsize=2";
	input << "\t}\n";
	input << "}";

	Vic2::Province theProvince("42", input);

	ASSERT_EQ(theProvince.getPopulation(), 3);
}


TEST(CK2World_ProvinceTests, getLiteracyWeightedPopulationDefaultsToZero)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	Vic2::Province theProvince("42", input);

	ASSERT_EQ(theProvince.getLiteracyWeightedPopulation(), 0);
}


TEST(CK2World_ProvinceTests, getLiteracyWeightedPopulationGivesTenPercentAtNoLiteracy)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\taristocrats={\n";
	input << "\t\tsize=100";
	input << "\t}\n";
	input << "}";

	Vic2::Province theProvince("42", input);

	ASSERT_EQ(theProvince.getLiteracyWeightedPopulation("aristocrats"), 10);
}


TEST(CK2World_ProvinceTests, getLiteracyWeightedPopulationGivesOneHundredPercentAtFullLiteracy)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\taristocrats={\n";
	input << "\t\tsize=100";
	input << "\t\tliteracy=1.0";
	input << "\t}\n";
	input << "}";

	Vic2::Province theProvince("42", input);

	ASSERT_EQ(theProvince.getLiteracyWeightedPopulation("aristocrats"), 100);
}


TEST(CK2World_ProvinceTests, getLiteracyWeightedPopulationDiscriminatesByPopType)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\taristocrats={\n";
	input << "\t\tsize=100";
	input << "\t\tliteracy=1.0";
	input << "\t}\n";
	input << "\tartisans={\n";
	input << "\t\tsize=100";
	input << "\t\tliteracy=0.5";
	input << "\t}\n";
	input << "}";

	Vic2::Province theProvince("42", input);

	ASSERT_EQ(theProvince.getLiteracyWeightedPopulation("aristocrats"), 100);
}


TEST(CK2World_ProvinceTests, getLiteracyWeightedPopulationGivesAllPopsWhenNoTypeSpecified)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\taristocrats={\n";
	input << "\t\tsize=100";
	input << "\t\tliteracy=1.0";
	input << "\t}\n";
	input << "\tartisans={\n";
	input << "\t\tsize=100";
	input << "\t\tliteracy=0.5";
	input << "\t}\n";
	input << "}";

	Vic2::Province theProvince("42", input);

	ASSERT_EQ(theProvince.getLiteracyWeightedPopulation(), 155);
}


TEST(CK2World_ProvinceTests, getPercentageWithCulturesReturnsZeroIfNoPops)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	Vic2::Province theProvince("42", input);

	std::set<std::string> cultures;
	ASSERT_EQ(theProvince.getPercentageWithCultures(cultures), 0.0);
}


TEST(CK2World_ProvinceTests, getPercentageWithCulturesReturnsZeroIfNoCulturesSpecified)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\taristocrats={\n";
	input << "\t\tsize=100";
	input << "\t\tculture=religion";
	input << "\t}\n";
	input << "}";

	Vic2::Province theProvince("42", input);

	std::set<std::string> cultures;
	ASSERT_EQ(theProvince.getPercentageWithCultures(cultures), 0.0);
}


TEST(CK2World_ProvinceTests, getPercentageWithCulturesReturnsZeroIfNoCulturesmatch)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\taristocrats={\n";
	input << "\t\tsize=100";
	input << "\t\tculture=religion";
	input << "\t}\n";
	input << "}";

	Vic2::Province theProvince("42", input);

	std::set<std::string> cultures;
	cultures.insert("wrong_culture");
	ASSERT_EQ(theProvince.getPercentageWithCultures(cultures), 0.0);
}


TEST(CK2World_ProvinceTests, getPercentageWithCulturesReturnsMatchedPercent)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\taristocrats={\n";
	input << "\t\tsize=100";
	input << "\t\tculture=religion";
	input << "\t}\n";
	input << "\taristocrats={\n";
	input << "\t\tsize=100";
	input << "\t\tculture2=religion";
	input << "\t}\n";
	input << "}";

	Vic2::Province theProvince("42", input);

	std::set<std::string> cultures;
	cultures.insert("culture");
	ASSERT_EQ(theProvince.getPercentageWithCultures(cultures), 0.5);
}


TEST(CK2World_ProvinceTests, getPercentageWithCulturesCanMapMultipleCultures)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\taristocrats={\n";
	input << "\t\tsize=100";
	input << "\t\tculture=religion";
	input << "\t}\n";
	input << "\taristocrats={\n";
	input << "\t\tsize=100";
	input << "\t\tanother_culture=religion";
	input << "\t}\n";
	input << "}";

	Vic2::Province theProvince("42", input);

	std::set<std::string> cultures;
	cultures.insert("culture");
	cultures.insert("another_culture");
	ASSERT_EQ(theProvince.getPercentageWithCultures(cultures), 1.0);
}


TEST(CK2World_ProvinceTests, navalBaseLevelDefaultsToZero)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	Vic2::Province theProvince("42", input);

	ASSERT_EQ(theProvince.getNavalBaseLevel(), 0);
}


TEST(CK2World_ProvinceTests, navalBaseLevelCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\tnaval_base =\n";
	input << "\t{\n";
	input << "  6.000 6.000 }\n";
	input << "}";

	Vic2::Province theProvince("42", input);

	ASSERT_EQ(theProvince.getNavalBaseLevel(), 6);
}


TEST(CK2World_ProvinceTests, railLevelDefaultsToZero)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	Vic2::Province theProvince("42", input);

	ASSERT_EQ(theProvince.getRailLevel(), 0);
}


TEST(CK2World_ProvinceTests, railLevelCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\trailroad =\n";
	input << "\t{\n";
	input << "  5.000 5.000 }\n";
	input << "}";

	Vic2::Province theProvince("42", input);

	ASSERT_EQ(theProvince.getRailLevel(), 5);
}