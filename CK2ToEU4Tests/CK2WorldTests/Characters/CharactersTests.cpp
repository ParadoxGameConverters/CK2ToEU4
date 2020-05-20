#include "../../CK2ToEU4/Source/CK2World/Characters/Character.h"
#include "../../CK2ToEU4/Source/CK2World/Characters/Characters.h"
#include "../../CK2ToEU4/Source/CK2World/Dynasties/Dynasties.h"
#include "../../CK2ToEU4/Source/CK2World/Dynasties/Dynasty.h"
#include "../../CK2ToEU4/Source/CK2World/Provinces/Barony.h"
#include "../../CK2ToEU4/Source/CK2World/Provinces/Provinces.h"
#include "../../CK2ToEU4/Source/CK2World/Titles/Title.h"
#include "../../CK2ToEU4/Source/CK2World/Titles/Titles.h"
#include "gtest/gtest.h"
#include <sstream>

TEST(CK2World_CharactersTests, CharactersDefaultToEmpty)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	const CK2::Characters characters(input);

	ASSERT_EQ(characters.getCharacters().empty(), true);
}

TEST(CK2World_CharactersTests, charactersCanBeLoaded)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "42={}\n";
	input << "43={}\n";
	input << "}";

	const CK2::Characters characters(input);
	const auto& characterItr = characters.getCharacters().find(42);
	const auto& characterItr2 = characters.getCharacters().find(43);

	ASSERT_EQ(characterItr->first, 42);
	ASSERT_EQ(characterItr->second->getID(), 42);
	ASSERT_EQ(characterItr2->first, 43);
	ASSERT_EQ(characterItr2->second->getID(), 43);
}

TEST(CK2World_CharactersTests, charactersDynastyLinkDefaultsToNull)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "42={dyn=34}\n";
	input << "}";

	const CK2::Characters characters(input);
	const auto& characterItr = characters.getCharacters().find(42);
	const auto& dynasty = characterItr->second->getDynasty();

	ASSERT_FALSE(dynasty.second);
}

TEST(CK2World_CharactersTests, charactersDynastyLinkCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "42={dnt=34}\n";
	input << "}";
	CK2::Characters characters(input);

	std::stringstream input2;
	input2 << "=\n";
	input2 << "{\n";
	input2 << "34={name=\"von Test\"}\n";
	input2 << "}";
	CK2::Dynasties dynasties(input2);

	characters.linkDynasties(dynasties);
	const auto& characterItr = characters.getCharacters().find(42);
	const auto& dynasty = characterItr->second->getDynasty();

	ASSERT_TRUE(dynasty.second);
	ASSERT_EQ(dynasty.second->getName(), "von Test");
}

TEST(CK2World_CharactersTests, charactersDynastyLinkCannotBeSetThrowsWarning)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "42={dnt=34}\n";
	input << "}";
	CK2::Characters characters(input);

	std::stringstream input2;
	input2 << "=\n";
	input2 << "{\n";
	input2 << "33={name=\"von Test\"}\n";
	input2 << "}";
	CK2::Dynasties dynasties(input2);

	std::stringstream log;
	auto stdOutBuf = std::cout.rdbuf();
	std::cout.rdbuf(log.rdbuf());

	characters.linkDynasties(dynasties);

	std::cout.rdbuf(stdOutBuf);
	auto stringLog = log.str();
	auto newLine = stringLog.find_first_of('\n');
	stringLog = stringLog.substr(0, newLine);

	ASSERT_EQ(stringLog, " [WARNING] Dynasty ID: 34 has no definition!");
}


TEST(CK2World_CharactersTests, charactersLiegeLinkDefaultsToNull)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "42={lge=34}\n";
	input << "}";

	const CK2::Characters characters(input);
	const auto& characterItr = characters.getCharacters().find(42);
	const auto& liege = characterItr->second->getLiege();

	ASSERT_FALSE(liege.second);
}

TEST(CK2World_CharactersTests, charactersLiegeLinkCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "34={bn=\"Herr Flick\"}\n";
	input << "42={lge=34}\n";
	input << "}";
	CK2::Characters characters(input);

	characters.linkLiegesAndSpouses();
	const auto& characterItr = characters.getCharacters().find(42);
	const auto& liege = characterItr->second->getLiege();

	ASSERT_TRUE(liege.second);
	ASSERT_EQ(liege.second->getName(), "Herr Flick");
}

TEST(CK2World_CharactersTests, charactersLiegeLinkCannotBeSetThrowsWarning)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "34={bn=\"Herr Flick\"}\n";
	input << "42={lge=33}\n";
	input << "}";
	CK2::Characters characters(input);

	std::stringstream log;
	auto stdOutBuf = std::cout.rdbuf();
	std::cout.rdbuf(log.rdbuf());

	characters.linkLiegesAndSpouses();

	std::cout.rdbuf(stdOutBuf);
	auto stringLog = log.str();
	auto newLine = stringLog.find_first_of('\n');
	stringLog = stringLog.substr(0, newLine);

	ASSERT_EQ(stringLog, " [WARNING] Liege ID: 33 has no definition!");
}


TEST(CK2World_CharactersTests, charactersPrimaryTitleLinkDefaultsToNull)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "42={primary=\"k_test\"}\n";
	input << "}";

	const CK2::Characters characters(input);
	const auto& characterItr = characters.getCharacters().find(42);
	const auto& primary = characterItr->second->getPrimaryTitle();

	ASSERT_FALSE(primary.second);
}

TEST(CK2World_CharactersTests, charactersPrimaryTitleLinkCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "42={dmn={primary=\"k_test\"}}\n";
	input << "}";
	CK2::Characters characters(input);

	std::stringstream input2;
	input2 << "=\n";
	input2 << "{\n";
	input2 << "k_test={}\n";
	input2 << "}";
	CK2::Titles titles(input2);

	characters.linkPrimaryTitles(titles);
	const auto& characterItr = characters.getCharacters().find(42);
	const auto& primary = characterItr->second->getPrimaryTitle().second->getTitle();

	ASSERT_TRUE(primary.second);
	ASSERT_EQ(primary.second->getName(), "k_test");
}

TEST(CK2World_CharactersTests, charactersPrimaryTitleLinkCannotBeSetThrowsWarning)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "42={dmn={primary=\"k_test\"}}\n";
	input << "}";
	CK2::Characters characters(input);

	std::stringstream input2;
	input2 << "=\n";
	input2 << "{\n";
	input2 << "c_test={}\n";
	input2 << "}";
	CK2::Titles titles(input2);

	std::stringstream log;
	auto stdOutBuf = std::cout.rdbuf();
	std::cout.rdbuf(log.rdbuf());

	characters.linkPrimaryTitles(titles);

	std::cout.rdbuf(stdOutBuf);
	auto stringLog = log.str();
	auto newLine = stringLog.find_first_of('\n');
	stringLog = stringLog.substr(0, newLine);

	ASSERT_EQ(stringLog, " [WARNING] Primary title ID: k_test has no definition!");
}

TEST(CK2World_CharactersTests, charactersPrimaryBaseTitleLinkDefaultsToNull)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "42=\n";
	input << "\t{dmn=\n";
	input << "\t\t{primary =\n";
	input << "\t\t\t{ ";
	input << "\t\t\ttitle = \"k_test\"\n";
	input << "\t\t\tbase_title = \"c_skyward\"\n";
	input << "\t\t\t}\n";
	input << "\t\t}\n";
	input << "\t}\n";
	input << "}\n";
	CK2::Characters characters(input);

	const auto& characterItr = characters.getCharacters().find(42);
	const auto& base = characterItr->second->getPrimaryTitle().second->getBaseTitle();

	ASSERT_FALSE(base.second);
}

TEST(CK2World_CharactersTests, charactersPrimaryBaseTitleLinkCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "42=\n";
	input << "\t{dmn=\n";
	input << "\t\t{primary =\n";
	input << "\t\t\t{ ";
	input << "\t\t\ttitle = \"k_test\"\n";
	input << "\t\t\tbase_title = \"c_skyward\"\n";
	input << "\t\t\t}\n";
	input << "\t\t}\n";
	input << "\t}\n";
	input << "}\n";
	CK2::Characters characters(input);

	std::stringstream input2;
	input2 << "=\n";
	input2 << "{\n";
	input2 << "k_test={}\n";
	input2 << "c_skyward={}\n";
	input2 << "}";
	CK2::Titles titles(input2);

	characters.linkPrimaryTitles(titles);
	const auto& characterItr = characters.getCharacters().find(42);
	const auto& base = characterItr->second->getPrimaryTitle().second->getBaseTitle();

	ASSERT_TRUE(base.second);
	ASSERT_EQ(base.second->getName(), "c_skyward");
}

TEST(CK2World_CharactersTests, charactersPrimaryBaseTitleLinkCannotBeSetThrowsWarning)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "42=\n";
	input << "\t{dmn=\n";
	input << "\t\t{primary =\n";
	input << "\t\t\t{ ";
	input << "\t\t\ttitle = \"k_test\"\n";
	input << "\t\t\tbase_title = \"c_skyward\"\n";
	input << "\t\t\t}\n";
	input << "\t\t}\n";
	input << "\t}\n";
	input << "}\n";
	CK2::Characters characters(input);

	std::stringstream input2;
	input2 << "=\n";
	input2 << "{\n";
	input2 << "k_test={}\n";
	input2 << "b_skyward={}\n";
	input2 << "}";
	CK2::Titles titles(input2);

	std::stringstream log;
	auto stdOutBuf = std::cout.rdbuf();
	std::cout.rdbuf(log.rdbuf());

	characters.linkPrimaryTitles(titles);

	std::cout.rdbuf(stdOutBuf);
	auto stringLog = log.str();
	auto newLine = stringLog.find_first_of('\n');
	stringLog = stringLog.substr(0, newLine);

	ASSERT_EQ(stringLog, " [WARNING] Base title ID: c_skyward has no definition!");
}

TEST(CK2World_CharactersTests, charactersCapitalLinkDefaultsToNull)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "42=\n";
	input << "\t{dmn=\n";
	input << "\t\t{capital = \"b_test\"\n";
	input << "\t\t}\n";
	input << "\t}\n";
	input << "}\n";
	CK2::Characters characters(input);

	const auto& characterItr = characters.getCharacters().find(42);
	const auto& capital = characterItr->second->getCapital();

	ASSERT_FALSE(capital.second);
}

TEST(CK2World_CharactersTests, charactersCapitalLinkCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "42=\n";
	input << "\t{dmn=\n";
	input << "\t\t{capital = \"b_test\"\n";
	input << "\t\t}\n";
	input << "\t}\n";
	input << "}\n";
	CK2::Characters characters(input);

	std::stringstream input2;
	input2 << "=\n";
	input2 << "{\n";
	input2 << "17={\n";
	input2 << "\tb_test={}\n";
	input2 << "\t}\n";
	input2 << "}";
	CK2::Provinces provinces(input2);

	characters.linkCapitals(provinces);
	const auto& characterItr = characters.getCharacters().find(42);
	const auto& capital = characterItr->second->getCapital();

	ASSERT_TRUE(capital.second);
	ASSERT_EQ(capital.second->getName(), "b_test");
}

TEST(CK2World_CharactersTests, charactersCapitalLinkCannotBeSetThrowsWarning)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "42=\n";
	input << "\t{dmn=\n";
	input << "\t\t{capital = \"b_test\"\n";
	input << "\t\t}\n";
	input << "\t}\n";
	input << "}\n";
	CK2::Characters characters(input);

	std::stringstream input2;
	input2 << "=\n";
	input2 << "{\n";
	input2 << "42={\n";
	input2 << "\tb_test2={}\n";
	input2 << "\t}\n";
	input2 << "}";
	CK2::Provinces provinces(input2);

	std::stringstream log;
	auto stdOutBuf = std::cout.rdbuf();
	std::cout.rdbuf(log.rdbuf());

	characters.linkCapitals(provinces);

	std::cout.rdbuf(stdOutBuf);
	auto stringLog = log.str();
	auto newLine = stringLog.find_first_of('\n');
	stringLog = stringLog.substr(0, newLine);

	ASSERT_EQ(stringLog, " [WARNING] Capital barony ID: b_test has no definition!");
}


TEST(CK2World_CharactersTests, charactersMotherLinkDefaultsToNull)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "42={}\n";
	input << "\t}\n";
	input << "}\n";
	CK2::Characters characters(input);

	const auto& characterItr = characters.getCharacters().find(42);
	const auto& mother = characterItr->second->getMother();

	ASSERT_FALSE(mother.second);
}

TEST(CK2World_CharactersTests, charactersFatherLinkDefaultsToNull)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "42={}\n";
	input << "\t}\n";
	input << "}\n";
	CK2::Characters characters(input);

	const auto& characterItr = characters.getCharacters().find(42);
	const auto& father = characterItr->second->getFather();

	ASSERT_FALSE(father.second);
}

TEST(CK2World_CharactersTests, charactersParentLinksCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "42={mot=20\nfat=21}\n";
	input << "20={}\n";
	input << "21={}\n";
	input << "\t}\n";
	input << "}\n";
	CK2::Characters characters(input);

	characters.linkMothersAndFathers();
	const auto& characterItr = characters.getCharacters().find(42);
	const auto& mother = characterItr->second->getMother();
	const auto& father = characterItr->second->getFather();

	ASSERT_TRUE(mother.second);
	ASSERT_TRUE(father.second);
	ASSERT_EQ(mother.second->getID(), 20);
	ASSERT_EQ(father.second->getID(), 21);
}

TEST(CK2World_CharactersTests, charactersFatherLinkCannotBeSetThrowsWarning)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "42={mot=20\nfat=21}\n";
	input << "20={}\n";
	input << "22={}\n";
	input << "\t}\n";
	input << "}\n";
	CK2::Characters characters(input);

	std::stringstream log;
	auto stdOutBuf = std::cout.rdbuf();
	std::cout.rdbuf(log.rdbuf());

	characters.linkMothersAndFathers();

	std::cout.rdbuf(stdOutBuf);
	auto stringLog = log.str();
	auto newLine = stringLog.find_first_of('\n');
	stringLog = stringLog.substr(0, newLine);

	ASSERT_EQ(stringLog, " [WARNING] Father ID: 21 has no definition!");
}

TEST(CK2World_CharactersTests, charactersMotherLinkCannotBeSetThrowsWarning)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "42={mot=19\nfat=22}\n";
	input << "20={}\n";
	input << "22={}\n";
	input << "\t}\n";
	input << "}\n";
	CK2::Characters characters(input);

	std::stringstream log;
	auto stdOutBuf = std::cout.rdbuf();
	std::cout.rdbuf(log.rdbuf());

	characters.linkMothersAndFathers();

	std::cout.rdbuf(stdOutBuf);
	auto stringLog = log.str();
	auto newLine = stringLog.find_first_of('\n');
	stringLog = stringLog.substr(0, newLine);

	ASSERT_EQ(stringLog, " [WARNING] Mother ID: 19 has no definition!");
}

TEST(CK2World_CharactersTests, charactersChildrenDefaultsToEmpty)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "42={}\n";
	input << "\t}\n";
	input << "}\n";
	CK2::Characters characters(input);

	const auto& characterItr = characters.getCharacters().find(42);
	const auto& children = characterItr->second->getChildren();

	ASSERT_TRUE(children.empty());
}

TEST(CK2World_CharactersTests, charactersChildrenLinksCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "42={mot=20}\n";
	input << "43={mot=20}\n";
	input << "20={}\n";
	input << "\t}\n";
	input << "}\n";
	CK2::Characters characters(input);

	characters.linkMothersAndFathers();
	const auto& characterItr = characters.getCharacters().find(20);
	const auto& children = characterItr->second->getChildren();

	ASSERT_EQ(children.size(), 2);
	ASSERT_EQ(children.find(42)->second->getID(), 42);
	ASSERT_EQ(children.find(43)->second->getID(), 43);
}
