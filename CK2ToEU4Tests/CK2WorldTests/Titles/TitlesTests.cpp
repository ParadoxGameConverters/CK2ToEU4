#include "../../CK2ToEU4/Source/CK2World/Characters/Character.h"
#include "../../CK2ToEU4/Source/CK2World/Characters/Characters.h"
#include "../../CK2ToEU4/Source/CK2World/Provinces/Province.h"
#include "../../CK2ToEU4/Source/CK2World/Provinces/Provinces.h"
#include "../../CK2ToEU4/Source/CK2World/Titles/Liege.h"
#include "../../CK2ToEU4/Source/CK2World/Titles/Title.h"
#include "../../CK2ToEU4/Source/CK2World/Titles/Titles.h"
#include "gtest/gtest.h"
#include <sstream>

// Function linkProvinces depends on provinceTitleMapper which is untestable due to
// disk access. We manually register provinces to titles instead of using that function
// so that coalesceProvinces and congregateProvinces can be tested.

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

TEST(CK2World_TitlesTests, liegeVassalsDefaultToEmpty)
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

	ASSERT_TRUE(liegeTitle.second->getVassals().empty());
}

TEST(CK2World_TitlesTests, liegeVassalsCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "c_title={liege=d_liege}\n";
	input << "d_liege={}\n";
	input << "}";
	CK2::Titles titles(input);

	titles.linkLiegePrimaryTitles();
	titles.linkVassals();
	const auto& titleItr = titles.getTitles().find("c_title");
	const auto& liegeTitle = titleItr->second->getLiege().second->getTitle();
	const auto& linktoSelf = liegeTitle.second->getVassals().find("c_title");

	ASSERT_FALSE(liegeTitle.second->getVassals().empty());
	ASSERT_EQ(linktoSelf->first, "c_title");
	ASSERT_EQ(linktoSelf->second->getName(), "c_title");
}

TEST(CK2World_TitlesTests, liegeDeJureVassalsDefaultToEmpty)
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
	const auto& liegeTitle = titleItr->second->getDeJureLiege().second->getTitle();

	ASSERT_TRUE(liegeTitle.second->getDeJureVassals().empty());
}

TEST(CK2World_TitlesTests, liegeDeJureVassalsCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "c_title={de_jure_liege=d_liege}\n";
	input << "d_liege={}\n";
	input << "}";
	CK2::Titles titles(input);

	titles.linkLiegePrimaryTitles();
	titles.linkVassals();
	const auto& titleItr = titles.getTitles().find("c_title");
	const auto& liegeTitle = titleItr->second->getDeJureLiege().second->getTitle();
	const auto& linktoSelf = liegeTitle.second->getDeJureVassals().find("c_title");

	ASSERT_FALSE(liegeTitle.second->getDeJureVassals().empty());
	ASSERT_EQ(linktoSelf->first, "c_title");
	ASSERT_EQ(linktoSelf->second->getName(), "c_title");
}

TEST(CK2World_TitlesTests, baseTitleTitleLinkDefaultsToNull)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "c_title={\n";
	input << "\tbase_title=c_base\n";
	input << "}\n";
	input << "}";

	const CK2::Titles titles(input);
	const auto& titleItr = titles.getTitles().find("c_title");
	const auto& base = titleItr->second->getBaseTitle().second->getTitle();

	ASSERT_FALSE(base.second);
}

TEST(CK2World_TitlesTests, baseTitleTitleLinkCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "c_title={\n";
	input << "\tbase_title=c_base\n";
	input << "}\n";
	input << "c_base={}\n";
	input << "}";
	CK2::Titles titles(input);

	titles.linkBaseTitles();
	const auto& titleItr = titles.getTitles().find("c_title");
	const auto& base = titleItr->second->getBaseTitle().second->getTitle();

	ASSERT_TRUE(base.second);
	ASSERT_EQ(base.second->getName(), "c_base");
}

TEST(CK2World_TitlesTests, baseTitleLinkCannotBeSetThrowsWarning)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "c_title={\n";
	input << "\tbase_title=c_base\n";
	input << "}\n";
	input << "c_base2={}\n";
	input << "}";
	CK2::Titles titles(input);

	std::stringstream log;
	auto stdOutBuf = std::cout.rdbuf();
	std::cout.rdbuf(log.rdbuf());

	titles.linkBaseTitles();

	std::cout.rdbuf(stdOutBuf);
	auto stringLog = log.str();
	auto newLine = stringLog.find_first_of("\n");
	stringLog = stringLog.substr(0, newLine);

	ASSERT_EQ(stringLog, "Base title title ID: c_base has no definition!");
}

TEST(CK2World_TitlesTests, baseTitleBaseTitleLinkDefaultsToNull)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "c_title={\n";
	input << "\tbase_title=c_base\n";
	input << "}\n";
	input << "}";

	const CK2::Titles titles(input);
	const auto& titleItr = titles.getTitles().find("c_title");
	const auto& base = titleItr->second->getBaseTitle().second->getBaseTitle();

	ASSERT_FALSE(base.second);
}

TEST(CK2World_TitlesTests, baseTitleBaseTitleLinkCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "c_title={\n";
	input << "\tbase_title={\n";
	input << "\t\ttitle=c_something\n";
	input << "\t\tbase_title=c_base\n";
	input << "\t}\n";
	input << "}\n";
	input << "c_base={}\n";
	input << "}";
	CK2::Titles titles(input);

	titles.linkBaseTitles();
	const auto& titleItr = titles.getTitles().find("c_title");
	const auto& base = titleItr->second->getBaseTitle().second->getBaseTitle();

	ASSERT_TRUE(base.second);
	ASSERT_EQ(base.second->getName(), "c_base");
}

TEST(CK2World_TitlesTests, baseTitleBaseTitleLinkCannotBeSetThrowsWarning)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "c_title={\n";
	input << "\tbase_title={\n";
	input << "\t\ttitle=c_something\n";
	input << "\t\tbase_title=c_base\n";
	input << "\t}\n";
	input << "}\n";
	input << "c_something={}\n";
	input << "}";
	CK2::Titles titles(input);

	std::stringstream log;
	auto stdOutBuf = std::cout.rdbuf();
	std::cout.rdbuf(log.rdbuf());

	titles.linkBaseTitles();

	std::cout.rdbuf(stdOutBuf);
	auto stringLog = log.str();
	auto newLine = stringLog.find_first_of("\n");
	stringLog = stringLog.substr(0, newLine);

	ASSERT_EQ(stringLog, "Base title base title ID: c_base has no definition!");
}

TEST(CK2World_TitlesTests, vassalsDefaultToEmpty)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "c_test1={liege=d_test}\n";
	input << "c_test2={liege=d_test}\n";
	input << "c_test3={liege=d_test}\n";
	input << "d_test={}\n";
	input << "}";
	CK2::Titles theTitles(input);
	const auto& title4 = theTitles.getTitles().find("d_test");

	ASSERT_TRUE(title4->second->getVassals().empty());
}

TEST(CK2World_TitlesTests, vassalsCanBeLinked)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "c_test1={liege=d_test}\n";
	input << "c_test2={liege=d_test}\n";
	input << "c_test3={liege=d_test}\n";
	input << "d_test={}\n";
	input << "}";
	CK2::Titles theTitles(input);
	theTitles.linkLiegePrimaryTitles();
	theTitles.linkVassals();

	const auto& title4 = theTitles.getTitles().find("d_test");
	ASSERT_EQ(title4->second->getVassals().size(), 3);
}

TEST(CK2World_TitlesTests, provincesCanBeCoalesced)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "c_test1={liege=d_test}\n";
	input << "c_test2={liege=d_test}\n";
	input << "c_test3={liege=d_test}\n";
	input << "d_test={}\n";
	input << "}";
	CK2::Titles theTitles(input);
	theTitles.linkLiegePrimaryTitles();
	theTitles.linkVassals();

	std::stringstream input2;
	input2 << "=\n";
	input2 << "{\n";
	input2 << "42={}\n";
	input2 << "43={}\n";
	input2 << "44={}\n";
	input2 << "50={}\n";
	input2 << "}";
	const CK2::Provinces theProvinces(input2);

	const auto& province42 = theProvinces.getProvinces().find(42);
	const auto& province43 = theProvinces.getProvinces().find(43);
	const auto& province44 = theProvinces.getProvinces().find(44);
	const auto& province50 = theProvinces.getProvinces().find(50);
	const auto& title1 = theTitles.getTitles().find("c_test1");
	const auto& title2 = theTitles.getTitles().find("c_test2");
	const auto& title3 = theTitles.getTitles().find("c_test3");
	const auto& title4 = theTitles.getTitles().find("d_test");
	title1->second->registerProvince(std::pair(province42->first, province42->second));
	title2->second->registerProvince(std::pair(province43->first, province43->second));
	title3->second->registerProvince(std::pair(province44->first, province44->second));
	title4->second->registerProvince(std::pair(province50->first, province50->second));

	const auto& coalescedProvinces = title4->second->coalesceProvinces();

	ASSERT_EQ(title1->second->getProvinces().size(), 1);
	ASSERT_EQ(title2->second->getProvinces().size(), 1);
	ASSERT_EQ(title3->second->getProvinces().size(), 1);
	ASSERT_EQ(title4->second->getProvinces().size(), 1);

	ASSERT_EQ(coalescedProvinces.size(), 4);
	ASSERT_EQ(coalescedProvinces.find(42)->second->getID(), 42);
	ASSERT_EQ(coalescedProvinces.find(43)->second->getID(), 43);
	ASSERT_EQ(coalescedProvinces.find(44)->second->getID(), 44);
	ASSERT_EQ(coalescedProvinces.find(50)->second->getID(), 50);
}

TEST(CK2World_TitlesTests, provincesCanBeCongregated)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "c_test1={liege=d_test}\n";
	input << "c_test2={liege=d_test}\n";
	input << "c_test3={liege=d_test}\n";
	input << "d_test={}\n";
	input << "}";
	CK2::Titles theTitles(input);
	theTitles.linkLiegePrimaryTitles();
	theTitles.linkVassals();

	std::stringstream input2;
	input2 << "=\n";
	input2 << "{\n";
	input2 << "42={}\n";
	input2 << "43={}\n";
	input2 << "44={}\n";
	input2 << "50={}\n";
	input2 << "}";
	const CK2::Provinces theProvinces(input2);

	const auto& province42 = theProvinces.getProvinces().find(42);
	const auto& province43 = theProvinces.getProvinces().find(43);
	const auto& province44 = theProvinces.getProvinces().find(44);
	const auto& province50 = theProvinces.getProvinces().find(50);
	const auto& title1 = theTitles.getTitles().find("c_test1");
	const auto& title2 = theTitles.getTitles().find("c_test2");
	const auto& title3 = theTitles.getTitles().find("c_test3");
	const auto& title4 = theTitles.getTitles().find("d_test");
	title1->second->registerProvince(std::pair(province42->first, province42->second));
	title2->second->registerProvince(std::pair(province43->first, province43->second));
	title3->second->registerProvince(std::pair(province44->first, province44->second));
	title4->second->registerProvince(std::pair(province50->first, province50->second));

	std::map<std::string, std::shared_ptr<CK2::Title>> independentTitles;

	title4->second->congregateProvinces(independentTitles);
	const auto& dutchyProvinces = title4->second->getProvinces();

	ASSERT_EQ(dutchyProvinces.size(), 4);
	ASSERT_EQ(dutchyProvinces.find(42)->second->getID(), 42);
	ASSERT_EQ(dutchyProvinces.find(43)->second->getID(), 43);
	ASSERT_EQ(dutchyProvinces.find(44)->second->getID(), 44);
	ASSERT_EQ(dutchyProvinces.find(50)->second->getID(), 50);
}

TEST(CK2World_TitlesTests, provincesCanBeCongregatedExceptNonIndependents)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "c_test1={liege=d_test}\n";
	input << "c_test2={liege=d_test}\n";
	input << "c_test3={liege=d_test}\n";
	input << "d_test={}\n";
	input << "}";
	CK2::Titles theTitles(input);
	theTitles.linkLiegePrimaryTitles();
	theTitles.linkVassals();

	std::stringstream input2;
	input2 << "=\n";
	input2 << "{\n";
	input2 << "42={}\n";
	input2 << "43={}\n";
	input2 << "44={}\n";
	input2 << "50={}\n";
	input2 << "}";
	const CK2::Provinces theProvinces(input2);

	const auto& province42 = theProvinces.getProvinces().find(42);
	const auto& province43 = theProvinces.getProvinces().find(43);
	const auto& province44 = theProvinces.getProvinces().find(44);
	const auto& province50 = theProvinces.getProvinces().find(50);
	const auto& title1 = theTitles.getTitles().find("c_test1");
	const auto& title2 = theTitles.getTitles().find("c_test2");
	const auto& title3 = theTitles.getTitles().find("c_test3");
	const auto& title4 = theTitles.getTitles().find("d_test");
	title1->second->registerProvince(std::pair(province42->first, province42->second));
	title2->second->registerProvince(std::pair(province43->first, province43->second));
	title3->second->registerProvince(std::pair(province44->first, province44->second));
	title4->second->registerProvince(std::pair(province50->first, province50->second));

	std::map<std::string, std::shared_ptr<CK2::Title>> independentTitles;
	independentTitles.insert(std::pair(title1->first, title1->second));

	title4->second->congregateProvinces(independentTitles);
	const auto& dutchyProvinces = title4->second->getProvinces();

	ASSERT_EQ(dutchyProvinces.size(), 3);
	ASSERT_EQ(dutchyProvinces.count(42), 0);
}

TEST(CK2World_TitlesTests, revoltsCanBeMerged)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "c_test1={liege=d_test}\n";
	input << "c_test2={liege=d_test}\n";
	input << "c_revolter={liege=\n{\ntitle=\"d_revolt_dynamic\"\nbase_title=\"d_test\"\n}\n}\n";
	input << "d_revolt_dynamic=\n{\nmajor_revolt=yes\nbase_title=d_test\n}\n";
	input << "d_test={}\n";
	input << "}";
	CK2::Titles theTitles(input);
	theTitles.linkLiegePrimaryTitles();
	theTitles.linkVassals();
	theTitles.linkBaseTitles();
	theTitles.mergeRevolts();

	const auto& title3 = theTitles.getTitles().find("c_revolter");
	const auto& title4 = theTitles.getTitles().find("d_test");
	const auto& title5 = theTitles.getTitles().find("d_revolt_dynamic");

	// revolter has nominal liege
	ASSERT_EQ(title3->second->getLiege().first, "d_test");
	// revolt has been merged
	ASSERT_EQ(title4->second->getVassals().size(), 3);
	// revolt doesn't exist
	ASSERT_TRUE(title5->first.empty());
}
