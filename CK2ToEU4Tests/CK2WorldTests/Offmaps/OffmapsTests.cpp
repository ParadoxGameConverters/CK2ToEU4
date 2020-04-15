#include "../../CK2ToEU4/Source/CK2World/Offmaps/Offmap.h"
#include "../../CK2ToEU4/Source/CK2World/Offmaps/Offmaps.h"
#include "gtest/gtest.h"
#include <sstream>

TEST(CK2World_OffmapsTests, OffmapsDefaultToEmpty)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	const CK2::Offmaps offmaps(input);

	ASSERT_TRUE(offmaps.getOffmaps().empty());
}

TEST(CK2World_OffmapsTests, OffmapsCanBeLoaded)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "42={}\n";
	input << "43={}\n";
	input << "}";

	const CK2::Offmaps offmaps(input);
	const auto& offmap = offmaps.getOffmaps().find(42);
	const auto& offmap2 = offmaps.getOffmaps().find(43);

	ASSERT_EQ(offmaps.getOffmaps().size(), 2);
	ASSERT_EQ(offmap->first, 42);
	ASSERT_EQ(offmap2->first, 43);
}

TEST(CK2World_OffmapsTests, OffmapsReturnsFalseForNoChina)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "42={}\n";
	input << "43={}\n";
	input << "}";
	const CK2::Offmaps offmaps(input);

	ASSERT_FALSE(offmaps.getChina());
}

TEST(CK2World_OffmapsTests, OffmapsReturnsChinaForChina)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "42={type=\"offmap_china\"}\n";
	input << "43={}\n";
	input << "}";
	const CK2::Offmaps offmaps(input);

	ASSERT_TRUE(offmaps.getChina());
	ASSERT_EQ(offmaps.getChina()->first, 42);
	ASSERT_EQ(offmaps.getChina()->second->getType(), "offmap_china");
}
