#include "../CK2ToEU4/Source/CK2World/Characters/Character.h"
#include "../CK2ToEU4/Source/CK2World/Titles/Liege.h"
#include "gtest/gtest.h"
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

TEST(CK2World_CharacterTests, pietyCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\tpiety=17.43";
	input << "}";

	const CK2::Character theCharacter(input, 42);

	ASSERT_NEAR(theCharacter.getPiety(), 17.43, 0.01);
}

TEST(CK2World_CharacterTests, PietyDefaultsToZero)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	const CK2::Character theCharacter(input, 42);

	ASSERT_EQ(theCharacter.getPiety(), 0);
}

TEST(CK2World_CharacterTests, prestigeCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\tprs=17.43";
	input << "}";

	const CK2::Character theCharacter(input, 42);

	ASSERT_NEAR(theCharacter.getPrestige(), 17.43, 0.01);
}

TEST(CK2World_CharacterTests, PrestigeDefaultsToZero)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	const CK2::Character theCharacter(input, 42);

	ASSERT_EQ(theCharacter.getPrestige(), 0);
}

TEST(CK2World_CharacterTests, wealthCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\twealth=17.43";
	input << "}";

	const CK2::Character theCharacter(input, 42);

	ASSERT_NEAR(theCharacter.getWealth(), 17.43, 0.01);
}

TEST(CK2World_CharacterTests, wealthDefaultsToZero)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	const CK2::Character theCharacter(input, 42);

	ASSERT_EQ(theCharacter.getWealth(), 0);
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

TEST(CK2World_CharacterTests, jobCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\tjob=\"job_chancellor\"";
	input << "}";

	const CK2::Character theCharacter(input, 42);

	ASSERT_EQ(theCharacter.getJob(), "job_chancellor");
}

TEST(CK2World_CharacterTests, jobDefaultsToBlank)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	const CK2::Character theCharacter(input, 42);

	ASSERT_TRUE(theCharacter.getJob().empty());
}

TEST(CK2World_CharacterTests, governmentCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\tgov=\"government\"";
	input << "}";

	const CK2::Character theCharacter(input, 42);

	ASSERT_EQ(theCharacter.getGovernment(), "government");
}

TEST(CK2World_CharacterTests, governmentDefaultsToBlank)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	const CK2::Character theCharacter(input, 42);

	ASSERT_TRUE(theCharacter.getGovernment().empty());
}

TEST(CK2World_CharacterTests, traitsDefaultsToEmpty)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	const CK2::Character theCharacter(input, 42);

	ASSERT_TRUE(theCharacter.getTraits().empty());
}

TEST(CK2World_CharacterTests, traitsCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\ttr = { 1 2 3 }\n";
	input << "}";

	const CK2::Character theCharacter(input, 42);

	ASSERT_EQ(theCharacter.getTraits().size(), 3);
	ASSERT_EQ(theCharacter.getTraits().count(1), 1);
	ASSERT_EQ(theCharacter.getTraits().count(2), 1);
	ASSERT_EQ(theCharacter.getTraits().count(3), 1);
}

TEST(CK2World_CharacterTests, femaleCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\tfem=yes\n";
	input << "}";

	const CK2::Character theCharacter(input, 42);

	ASSERT_TRUE(theCharacter.isFemale());
}

TEST(CK2World_CharacterTests, femaleDefaultsToFalse)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	const CK2::Character theCharacter(input, 42);

	ASSERT_FALSE(theCharacter.isFemale());
}

TEST(CK2World_CharacterTests, hostCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\thost = 17\n";
	input << "}";

	const CK2::Character theCharacter(input, 42);

	ASSERT_EQ(theCharacter.getHost(), 17);
}

TEST(CK2World_CharacterTests, hostDefaultsToZero)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	const CK2::Character theCharacter(input, 42);

	ASSERT_FALSE(theCharacter.getHost());
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

TEST(CK2World_CharacterTests, motherCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\tmot=123";
	input << "}";

	const CK2::Character theCharacter(input, 42);

	ASSERT_EQ(theCharacter.getMother().first, 123);
}

TEST(CK2World_CharacterTests, motherDefaultsToZero)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	const CK2::Character theCharacter(input, 42);

	ASSERT_EQ(theCharacter.getMother().first, 0);
}
TEST(CK2World_CharacterTests, fatherCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\tfat=123";
	input << "}";

	const CK2::Character theCharacter(input, 42);

	ASSERT_EQ(theCharacter.getFather().first, 123);
}

TEST(CK2World_CharacterTests, fatherDefaultsToZero)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	const CK2::Character theCharacter(input, 42);

	ASSERT_EQ(theCharacter.getFather().first, 0);
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

TEST(CK2World_CharacterTests, characterDomainLoadsSimplePrimaryTitle)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\tdmn={\n";
	input << "\tprimary=c_test\n";
	input << "\t}\n";
	input << "}";

	const CK2::Character theCharacter(input, 42);

	ASSERT_EQ(theCharacter.getPrimaryTitle().second->getTitle().first, "c_test");
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

TEST(CK2World_CharacterTests, skillsDefaultToZero)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	const CK2::Character theCharacter(input, 42);

	ASSERT_EQ(theCharacter.getSkills().diplomacy, 0);
	ASSERT_EQ(theCharacter.getSkills().martial, 0);
	ASSERT_EQ(theCharacter.getSkills().stewardship, 0);
	ASSERT_EQ(theCharacter.getSkills().intrigue, 0);
	ASSERT_EQ(theCharacter.getSkills().learning, 0);
}

TEST(CK2World_CharacterTests, skillsCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\tatt={1 2 3 4 5}";
	input << "}";

	const CK2::Character theCharacter(input, 42);

	ASSERT_EQ(theCharacter.getSkills().diplomacy, 1);
	ASSERT_EQ(theCharacter.getSkills().martial, 2);
	ASSERT_EQ(theCharacter.getSkills().stewardship, 3);
	ASSERT_EQ(theCharacter.getSkills().intrigue, 4);
	ASSERT_EQ(theCharacter.getSkills().learning, 5);
}
