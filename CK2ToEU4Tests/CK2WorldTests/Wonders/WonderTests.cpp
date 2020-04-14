#include "../CK2ToEU4/Source/CK2World/Wonders/Wonder.h"
#include "gtest/gtest.h"
#include <sstream>

TEST(CK2World_WonderTests, nameDefaultsToBlank)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	const CK2::Wonder wonder(input);

	ASSERT_TRUE(wonder.getName().empty());
}

TEST(CK2World_WonderTests, nameCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\tname=\"Numero Uno\"";
	input << "}";

	const CK2::Wonder wonder(input);

	ASSERT_EQ(wonder.getName(), "Numero Uno");
}

TEST(CK2World_WonderTests, typeDefaultsToBlank)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	const CK2::Wonder wonder(input);

	ASSERT_TRUE(wonder.getType().empty());
}

TEST(CK2World_WonderTests, typeCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\ttype=\"wonder_pagan_stones_stonehenge\"";
	input << "}";

	const CK2::Wonder wonder(input);

	ASSERT_EQ(wonder.getType(), "wonder_pagan_stones_stonehenge");
}

TEST(CK2World_WonderTests, ProvinceIDDefaultsToZero)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	const CK2::Wonder wonder(input);

	ASSERT_FALSE(wonder.getProvinceID());
}

TEST(CK2World_WonderTests, ProvinceIDCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\tprovince=23\n";
	input << "}";

	const CK2::Wonder wonder(input);

	ASSERT_EQ(wonder.getProvinceID(), 23);
}

TEST(CK2World_WonderTests, ActiveDefaultsToFalse)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	const CK2::Wonder wonder(input);

	ASSERT_FALSE(wonder.isActive());
}

TEST(CK2World_WonderTests, ActiveCanBeNotSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\tactive=no\n";
	input << "}";

	const CK2::Wonder wonder(input);

	ASSERT_FALSE(wonder.isActive());
}

TEST(CK2World_WonderTests, ActiveCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\tactive=yes\n";
	input << "}";

	const CK2::Wonder wonder(input);

	ASSERT_TRUE(wonder.isActive());
}

TEST(CK2World_WonderTests, StageDefaultsToZero)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	const CK2::Wonder wonder(input);

	ASSERT_FALSE(wonder.getStage());
}

TEST(CK2World_WonderTests, StageCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\tstage=3\n";
	input << "}";

	const CK2::Wonder wonder(input);

	ASSERT_EQ(wonder.getStage(), 3);
}

TEST(CK2World_WonderTests, TransferrableDefaultsToFalse)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	const CK2::Wonder wonder(input);

	ASSERT_FALSE(wonder.isTransferrable());
}

TEST(CK2World_WonderTests, TransferrableCanBeSetForStageThreeAndActive)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\tstage=3\n";
	input << "\tactive=yes\n";
	input << "}";

	const CK2::Wonder wonder(input);

	ASSERT_TRUE(wonder.isTransferrable());
}
