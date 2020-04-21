#include "../CK2ToEU4/Source/CK2World/Characters/Domain.h"
#include "gtest/gtest.h"
#include <sstream>

TEST(CK2World_DomainTests, capitalDefaultsToBlank)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	const CK2::Domain theDomain(input);

	ASSERT_TRUE(theDomain.getCapital().first.empty());
}

TEST(CK2World_DomainTests, capitalCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "capital=\"b_test\"";
	input << "}";

	const CK2::Domain theDomain(input);

	ASSERT_EQ(theDomain.getCapital().first, "b_test");
}

TEST(CK2World_DomainTests, primaryTitleDefaultsToBlank)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	const CK2::Domain theDomain(input);

	ASSERT_TRUE(theDomain.getPrimaryTitle().first.empty());
}

TEST(CK2World_DomainTests, simplePrimaryTitleCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\tprimary=\"b_test\"";
	input << "}";

	const CK2::Domain theDomain(input);

	ASSERT_EQ(theDomain.getPrimaryTitle().first, "b_test");
}

TEST(CK2World_DomainTests, complexPrimaryTitleCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\tprimary=\n";
	input << "\t{\n";
	input << "\ttitle=\"b_test\"\n";
	input << "\t}\n";
	input << "}";

	const CK2::Domain theDomain(input);

	ASSERT_EQ(theDomain.getPrimaryTitle().first, "b_test");
}
