#include "../CK2ToEU4/Source/CK2World/Offmaps/Offmap.h"
#include "gtest/gtest.h"
#include <sstream>

TEST(CK2World_OffmapTests, nameDefaultsToBlank)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	const CK2::Offmap offmap(input);

	ASSERT_TRUE(offmap.getName().empty());
}

TEST(CK2World_OffmapTests, nameCanBeSetFromLastName)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\tnames= {\"Numero Uno\" due tres quatro }\n";
	input << "}";

	const CK2::Offmap offmap(input);

	ASSERT_EQ(offmap.getName(), "quatro");
}

TEST(CK2World_OffmapTests, typeDefaultsToBlank)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	const CK2::Offmap offmap(input);

	ASSERT_TRUE(offmap.getType().empty());
}

TEST(CK2World_OffmapTests, typeCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\ttype=\"offmap_china\"";
	input << "}";

	const CK2::Offmap offmap(input);

	ASSERT_EQ(offmap.getType(), "offmap_china");
}

TEST(CK2World_OffmapTests, HolderDefaultsToZero)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	const CK2::Offmap offmap(input);

	ASSERT_FALSE(offmap.getHolder().first);
}

TEST(CK2World_OffmapTests, HolderCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\tholder=78\n";
	input << "}";

	const CK2::Offmap offmap(input);

	ASSERT_EQ(offmap.getHolder().first, 78);
}
