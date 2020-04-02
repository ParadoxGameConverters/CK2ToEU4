#include "gtest/gtest.h"
#include "../CK2ToEU4/Source/CK2World/Characters/Character.h"
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

	ASSERT_EQ(theCharacter.getDynasty(), 123);
}

TEST(CK2World_CharacterTests, dynastyDefaultsToZero)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	const CK2::Character theCharacter(input, 42);

	ASSERT_EQ(theCharacter.getDynasty(), 0);
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

	ASSERT_EQ(theCharacter.getLiege(), 123);
}

TEST(CK2World_CharacterTests, liegeDefaultsToZero)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	const CK2::Character theCharacter(input, 42);

	ASSERT_EQ(theCharacter.getLiege(), 0);
}
