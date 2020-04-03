#include "gtest/gtest.h"
#include "../CK2ToEU4/Source/CK2World/Characters/Character.h"
#include "../CK2ToEU4/Source/CK2World/Provinces/Barony.h"
#include <sstream>

TEST(CK2World_CharacterTests, IDCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	const CK2::Character theCharacter(input, 42);

	ASSERT_EQ(theCharacter.getID(), 42);
}

TEST(CK2World_CharacterTests, cultureCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\tcul=\"paradoxian\"";
	input << "}";

	const CK2::Character theCharacter(input, 42);

	ASSERT_EQ(theCharacter.getCulture(), "paradoxian");
}

TEST(CK2World_CharacterTests, cultureDefaultsToBlank)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	const CK2::Character theCharacter(input, 42);

	ASSERT_TRUE(theCharacter.getCulture().empty());
}

TEST(CK2World_CharacterTests, religionCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\trel=\"paradoxian\"";
	input << "}";

	const CK2::Character theCharacter(input, 42);

	ASSERT_EQ(theCharacter.getReligion(), "paradoxian");
}

TEST(CK2World_CharacterTests, religionDefaultsToBlank)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	const CK2::Character theCharacter(input, 42);

	ASSERT_TRUE(theCharacter.getReligion().empty());
}

TEST(CK2World_CharacterTests, nameCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\tbn=\"paradoxian\"";
	input << "}";

	const CK2::Character theCharacter(input, 42);

	ASSERT_EQ(theCharacter.getName(), "paradoxian");
}

TEST(CK2World_CharacterTests, nameDefaultsToBlank)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	const CK2::Character theCharacter(input, 42);

	ASSERT_TRUE(theCharacter.getName().empty());
}

TEST(CK2World_CharacterTests, dynastyCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\tdnt=123";
	input << "}";

	const CK2::Character theCharacter(input, 42);

	ASSERT_EQ(theCharacter.getDynasty().first, 123);
}

TEST(CK2World_CharacterTests, dynastyDefaultsToZero)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	const CK2::Character theCharacter(input, 42);

	ASSERT_EQ(theCharacter.getDynasty().first, 0);
}

TEST(CK2World_CharacterTests, birthDateCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\tb_d=\"2.2.2\"";
	input << "}";

	const CK2::Character theCharacter(input, 42);

	ASSERT_EQ(theCharacter.getBirthDate(), date("2.2.2"));
}

TEST(CK2World_CharacterTests, birthDateDefaultstoDefault)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	const CK2::Character theCharacter(input, 42);

	ASSERT_EQ(theCharacter.getBirthDate(), date("1.1.1"));
}

TEST(CK2World_CharacterTests, deathDateCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\td_d=\"2.2.2\"";
	input << "}";

	const CK2::Character theCharacter(input, 42);

	ASSERT_EQ(theCharacter.getDeathDate(), date("2.2.2"));
}

TEST(CK2World_CharacterTests, deathDateDefaultstoDefault)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	const CK2::Character theCharacter(input, 42);

	ASSERT_EQ(theCharacter.getDeathDate(), date("1.1.1"));
}

TEST(CK2World_CharacterTests, liegeCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\tlge=123";
	input << "}";

	const CK2::Character theCharacter(input, 42);

	ASSERT_EQ(theCharacter.getLiege().first, 123);
}

TEST(CK2World_CharacterTests, liegeDefaultsToZero)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	const CK2::Character theCharacter(input, 42);

	ASSERT_EQ(theCharacter.getLiege().first, 0);
}

TEST(CK2World_CharacterTests, spousesDefaultToEmpty)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	const CK2::Character theCharacter(input, 42);

	ASSERT_TRUE(theCharacter.getSpouses().empty());
}

TEST(CK2World_CharacterTests, spouseCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\tspouse=123";
	input << "}";

	const CK2::Character theCharacter(input, 42);

	ASSERT_FALSE(theCharacter.getSpouses().empty());
	ASSERT_EQ(theCharacter.getSpouses().begin()->first, 123);
}

TEST(CK2World_CharacterTests, multipleSpousesCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\tspouse=123";
	input << "\tspouse=456";
	input << "\tspouse=789";
	input << "}";

	const CK2::Character theCharacter(input, 42);

	ASSERT_FALSE(theCharacter.getSpouses().empty());
	ASSERT_EQ(theCharacter.getSpouses().size(), 3);
}

TEST(CK2World_CharacterTests, characterDomainLoadsBlank)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\tdmn={\n";
	input << "\t}\n";
	input << "}";

	const CK2::Character theCharacter(input, 42);
	
	ASSERT_TRUE(theCharacter.getPrimaryTitle().first.empty());
	ASSERT_TRUE(theCharacter.getCapital().first.empty());
}

TEST(CK2World_CharacterTests, characterDomainLoadsPrimaryTitle)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\tdmn={\n";
	input << "\tprimary=c_test\n";
	input << "\t}\n";
	input << "}";

	const CK2::Character theCharacter(input, 42);

	ASSERT_EQ(theCharacter.getPrimaryTitle().first, "c_test");
}

TEST(CK2World_CharacterTests, characterDomainLoadsCapital)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\tdmn={\n";
	input << "\tcapital=b_test\n";
	input << "\t}\n";
	input << "}";

	const CK2::Character theCharacter(input, 42);

	ASSERT_EQ(theCharacter.getCapital().first, "b_test");
}
