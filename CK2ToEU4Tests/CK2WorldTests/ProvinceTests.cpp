#include "gtest/gtest.h"
#include "../CK2ToEU4/Source/CK2World/Provinces/Province.h"
#include <sstream>

TEST(CK2World_ProvinceTests, IDCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	CK2::Province theProvince(input, 42);

	ASSERT_EQ(theProvince.getID(), 42);
}

TEST(CK2World_ProvinceTests, cultureCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\tculture=\"paradoxian\"";
	input << "}";

	CK2::Province theProvince(input, 42);

	ASSERT_EQ(theProvince.getCulture(), "paradoxian");
}

TEST(CK2World_ProvinceTests, cultureDefaultsToBlank)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	CK2::Province theProvince(input, 42);

	ASSERT_EQ(theProvince.getCulture(), "");
}

TEST(CK2World_ProvinceTests, religionCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\treligion=\"paradoxian\"";
	input << "}";

	CK2::Province theProvince(input, 42);

	ASSERT_EQ(theProvince.getReligion(), "paradoxian");
}

TEST(CK2World_ProvinceTests, religionDefaultsToBlank)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	CK2::Province theProvince(input, 42);

	ASSERT_EQ(theProvince.getReligion(), "");
}

TEST(CK2World_ProvinceTests, nameCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\tname=\"paradoxian\"";
	input << "}";

	CK2::Province theProvince(input, 42);

	ASSERT_EQ(theProvince.getName(), "paradoxian");
}

TEST(CK2World_ProvinceTests, nameDefaultsToBlank)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	CK2::Province theProvince(input, 42);

	ASSERT_EQ(theProvince.getName(), "");
}

TEST(CK2World_ProvinceTests, primarySettlementCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\tprimary_settlement=\"b_paradox\"";
	input << "}";

	CK2::Province theProvince(input, 42);

	ASSERT_EQ(theProvince.getPrimarySettlement(), "b_paradox");
}

TEST(CK2World_ProvinceTests, primarySettlementDefaultsToBlank)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	CK2::Province theProvince(input, 42);

	ASSERT_EQ(theProvince.getPrimarySettlement(), "");
}
