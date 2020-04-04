#include "gtest/gtest.h"
#include <sstream>
#include "../../CK2ToEU4/Source/CK2World/Titles/Titles.h"
#include "../../CK2ToEU4/Source/CK2World/Titles/Title.h"
#include "../../CK2ToEU4/Source/CK2World/Characters/Characters.h"
#include "../../CK2ToEU4/Source/CK2World/Characters/Character.h"
#include "../../CK2ToEU4/Source/CK2World/Titles/Liege.h"

TEST(CK2World_TitlesTests, titlesDefaultToEmpty)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	const CK2::Titles titles(input);

	ASSERT_TRUE(titles.getTitles().empty());
}

TEST(CK2World_TitlesTests, titleCanBeLoaded)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";	
	input << "c_test={}\n";
	input << "}";

	const CK2::Titles titles(input);

	ASSERT_FALSE(titles.getTitles().empty());
	ASSERT_EQ(titles.getTitles().begin()->first, "c_test");
	ASSERT_EQ(titles.getTitles().begin()->second->getName(), "c_test");
}

TEST(CK2World_TitlesTests, multipleTitlesCanBeLoaded)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "c_test={}\n";
	input << "d_test={}\n";
	input << "e_test={}\n";
	input << "d_dyn_reb_2869601={}\n";
	input << "e_dyn_2653319={}\n";
	input << "}";

	const CK2::Titles titles(input);

	ASSERT_FALSE(titles.getTitles().empty());
	ASSERT_EQ(titles.getTitles().size(), 5);
}

TEST(CK2World_TitlesTests, holderLinkDefaultsToNull)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "c_title={holder=34}\n";
	input << "}";

	const CK2::Titles titles(input);
	const auto& titleItr = titles.getTitles().find("c_title");
	const auto& holder = titleItr->second->getHolder();

	ASSERT_FALSE(holder.second);
}

TEST(CK2World_TitlesTests, holderLinkCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "c_title={holder=34}\n";
	input << "}";
	CK2::Titles titles(input);

	std::stringstream input2;
	input2 << "=\n";
	input2 << "{\n";
	input2 << "34={bn=\"von Test\"}\n";
	input2 << "}";
	CK2::Characters characters(input2);

	titles.linkHolders(characters);
	const auto& titleItr = titles.getTitles().find("c_title");
	const auto& holder = titleItr->second->getHolder();

	ASSERT_TRUE(holder.second);
	ASSERT_EQ(holder.second->getName(), "von Test");
}

TEST(CK2World_TitlesTests, holderLinkCannotBeSetThrowsWarning)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "c_title={holder=34}\n";
	input << "}";
	CK2::Titles titles(input);

	std::stringstream input2;
	input2 << "=\n";
	input2 << "{\n";
	input2 << "35={bn=\"von Test\"}\n";
	input2 << "}";
	CK2::Characters characters(input2);

	std::stringstream log;
	auto stdOutBuf = std::cout.rdbuf();
	std::cout.rdbuf(log.rdbuf());

	titles.linkHolders(characters);

	std::cout.rdbuf(stdOutBuf);
	auto stringLog = log.str();
	auto newLine = stringLog.find_first_of("\n");
	stringLog = stringLog.substr(0, newLine);

	ASSERT_EQ(stringLog, "Holder ID: 34 has no definition!");
}

TEST(CK2World_TitlesTests, liegePrimaryTitleLinkDefaultsToNull)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "c_title={liege=d_liege}\n";
	input << "}";

	const CK2::Titles titles(input);
	const auto& titleItr = titles.getTitles().find("c_title");
	const auto& liegeTitle = titleItr->second->getLiege().second->getTitle();

	ASSERT_FALSE(liegeTitle.second);
}

TEST(CK2World_TitlesTests, liegePrimaryTitleLinkCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "c_title={liege=d_liege}\n";
	input << "d_liege={}\n";
	input << "}";
	CK2::Titles titles(input);

	titles.linkLiegePrimaryTitles();
	const auto& titleItr = titles.getTitles().find("c_title");
	const auto& liegeTitle = titleItr->second->getLiege().second->getTitle();

	ASSERT_TRUE(liegeTitle.second);
	ASSERT_EQ(liegeTitle.second->getName(), "d_liege");
}

TEST(CK2World_TitlesTests, liegePrimaryTitleLinkCannotBeSetThrowsWarning)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "c_title={liege=d_liege}\n";
	input << "d_liege2={}\n";
	input << "}";
	CK2::Titles titles(input);

	std::stringstream log;
	auto stdOutBuf = std::cout.rdbuf();
	std::cout.rdbuf(log.rdbuf());

	titles.linkLiegePrimaryTitles();

	std::cout.rdbuf(stdOutBuf);
	auto stringLog = log.str();
	auto newLine = stringLog.find_first_of("\n");
	stringLog = stringLog.substr(0, newLine);

	ASSERT_EQ(stringLog, "Primary liege title ID: d_liege has no definition!");
}

TEST(CK2World_TitlesTests, liegeBaseTitleLinkDefaultsToNull)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "c_title={\n";
	input << "\tliege={\n";
	input << "\t\ttitle=d_liege\n";
	input << "\t\tbase_title=d_something\n";
	input << "\t}}\n";
	input << "}";

	const CK2::Titles titles(input);
	const auto& titleItr = titles.getTitles().find("c_title");
	const auto& liegeBaseTitle = titleItr->second->getLiege().second->getBaseTitle();

	ASSERT_FALSE(liegeBaseTitle.second);
}

TEST(CK2World_TitlesTests, liegeBaseTitleLinkCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "c_title={\n";
	input << "\tliege={\n";
	input << "\t\ttitle=d_liege\n";
	input << "\t\tbase_title=d_something\n";
	input << "\t}}\n";
	input << "d_liege={}\n";
	input << "d_something={}\n";
	input << "}";
	CK2::Titles titles(input);

	titles.linkLiegePrimaryTitles();
	const auto& titleItr = titles.getTitles().find("c_title");
	const auto& liegeBaseTitle = titleItr->second->getLiege().second->getBaseTitle();

	ASSERT_TRUE(liegeBaseTitle.second);
	ASSERT_EQ(liegeBaseTitle.second->getName(), "d_something");
}

TEST(CK2World_TitlesTests, liegeBaseTitleLinkCannotBeSetThrowsWarning)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "c_title={\n";
	input << "\tliege={\n";
	input << "\t\ttitle=d_liege\n";
	input << "\t\tbase_title=d_something\n";
	input << "\t}}\n";
	input << "d_liege={}\n";
	input << "d_something2={}\n";
	input << "}";
	CK2::Titles titles(input);

	std::stringstream log;
	auto stdOutBuf = std::cout.rdbuf();
	std::cout.rdbuf(log.rdbuf());

	titles.linkLiegePrimaryTitles();

	std::cout.rdbuf(stdOutBuf);
	auto stringLog = log.str();
	auto newLine = stringLog.find_first_of("\n");
	stringLog = stringLog.substr(0, newLine);

	ASSERT_EQ(stringLog, "Base liege title ID: d_something has no definition!");
}

TEST(CK2World_TitlesTests, DJliegePrimaryTitleLinkDefaultsToNull)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "c_title={de_jure_liege=d_liege}\n";
	input << "}";

	const CK2::Titles titles(input);
	const auto& titleItr = titles.getTitles().find("c_title");
	const auto& DJliegeTitle = titleItr->second->getDeJureLiege().second->getTitle();

	ASSERT_FALSE(DJliegeTitle.second);
}

TEST(CK2World_TitlesTests, DJliegePrimaryTitleLinkCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "c_title={de_jure_liege=d_liege}\n";
	input << "d_liege={}\n";
	input << "}";
	CK2::Titles titles(input);

	titles.linkLiegePrimaryTitles();
	const auto& titleItr = titles.getTitles().find("c_title");
	const auto& DJliegeTitle = titleItr->second->getDeJureLiege().second->getTitle();

	ASSERT_TRUE(DJliegeTitle.second);
	ASSERT_EQ(DJliegeTitle.second->getName(), "d_liege");
}

TEST(CK2World_TitlesTests, DJliegePrimaryTitleLinkCannotBeSetThrowsWarning)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "c_title={de_jure_liege=d_liege}\n";
	input << "d_liege2={}\n";
	input << "}";
	CK2::Titles titles(input);

	std::stringstream log;
	auto stdOutBuf = std::cout.rdbuf();
	std::cout.rdbuf(log.rdbuf());

	titles.linkLiegePrimaryTitles();

	std::cout.rdbuf(stdOutBuf);
	auto stringLog = log.str();
	auto newLine = stringLog.find_first_of("\n");
	stringLog = stringLog.substr(0, newLine);

	ASSERT_EQ(stringLog, "Primary DJ liege title ID: d_liege has no definition!");
}

TEST(CK2World_TitlesTests, DJliegeBaseTitleLinkDefaultsToNull)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "c_title={\n";
	input << "\tde_jure_liege={\n";
	input << "\t\ttitle=d_liege\n";
	input << "\t\tbase_title=d_something\n";
	input << "\t}}\n";
	input << "}";

	const CK2::Titles titles(input);
	const auto& titleItr = titles.getTitles().find("c_title");
	const auto& DJliegeBaseTitle = titleItr->second->getDeJureLiege().second->getBaseTitle();

	ASSERT_FALSE(DJliegeBaseTitle.second);
}

TEST(CK2World_TitlesTests, DJliegeBaseTitleLinkCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "c_title={\n";
	input << "\tde_jure_liege={\n";
	input << "\t\ttitle=d_liege\n";
	input << "\t\tbase_title=d_something\n";
	input << "\t}}\n";
	input << "d_liege={}\n";
	input << "d_something={}\n";
	input << "}";
	CK2::Titles titles(input);

	titles.linkLiegePrimaryTitles();
	const auto& titleItr = titles.getTitles().find("c_title");
	const auto& DJliegeBaseTitle = titleItr->second->getDeJureLiege().second->getBaseTitle();

	ASSERT_TRUE(DJliegeBaseTitle.second);
	ASSERT_EQ(DJliegeBaseTitle.second->getName(), "d_something");
}

TEST(CK2World_TitlesTests, DJliegeBaseTitleLinkCannotBeSetThrowsWarning)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "c_title={\n";
	input << "\tde_jure_liege={\n";
	input << "\t\ttitle=d_liege\n";
	input << "\t\tbase_title=d_something\n";
	input << "\t}}\n";
	input << "d_liege={}\n";
	input << "d_something2={}\n";
	input << "}";
	CK2::Titles titles(input);

	std::stringstream log;
	auto stdOutBuf = std::cout.rdbuf();
	std::cout.rdbuf(log.rdbuf());

	titles.linkLiegePrimaryTitles();

	std::cout.rdbuf(stdOutBuf);
	auto stringLog = log.str();
	auto newLine = stringLog.find_first_of("\n");
	stringLog = stringLog.substr(0, newLine);

	ASSERT_EQ(stringLog, "Base DJ liege title ID: d_something has no definition!");
}
