#include "../../CK2ToEU4/Source/CK2World/Provinces/Barony.h"
#include "../../CK2ToEU4/Source/CK2World/Provinces/Province.h"
#include "../../CK2ToEU4/Source/CK2World/Provinces/Provinces.h"
#include "../../CK2ToEU4/Source/CK2World/Wonders/Wonder.h"
#include "../../CK2ToEU4/Source/CK2World/Wonders/Wonders.h"
#include "gtest/gtest.h"
#include <sstream>
TEST(CK2World_ProvincesTests, provincesDefaultToEmpty)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}";

	const CK2::Provinces provinces(input);

	ASSERT_TRUE(provinces.getProvinces().empty());
}

TEST(CK2World_ProvincesTests, provinceCanBeLoaded)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "42={}\n";
	input << "}";

	const CK2::Provinces provinces(input);
	const auto& provinceItr = provinces.getProvinces().begin();

	ASSERT_EQ(provinceItr->first, 42);
	ASSERT_EQ(provinceItr->second->getID(), 42);
}

TEST(CK2World_ProvincesTests, invalidPrimarySettlementIsBlanked)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "42=\n";
	input << "\t{primary_settlement=\"---\"\n";
	input << "\t}\n";
	input << "}\n";
	CK2::Provinces provinces(input);

	provinces.linkPrimarySettlements();

	const auto& provinceItr = provinces.getProvinces().find(42);
	const auto& primary = provinceItr->second->getPrimarySettlement();

	ASSERT_TRUE(primary.first.empty());
}

TEST(CK2World_ProvincesTests, primarySettlementLinkDefaultsToNull)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "42=\n";
	input << "\t{primary_settlement=\"b_test\"\n";
	input << "\t}\n";
	input << "}\n";
	CK2::Provinces provinces(input);

	const auto& provinceItr = provinces.getProvinces().find(42);
	const auto& primary = provinceItr->second->getPrimarySettlement();

	ASSERT_FALSE(primary.second);
}

TEST(CK2World_ProvincesTests, primarySettlementLinkCanBeSet)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "42=\n";
	input << "\t{primary_settlement=\"b_test\"\n";
	input << "\tb_test={}\n";
	input << "\tb_test2={}\n";
	input << "\t}\n";
	input << "}\n";
	CK2::Provinces provinces(input);

	provinces.linkPrimarySettlements();

	const auto& provinceItr = provinces.getProvinces().find(42);
	const auto& primary = provinceItr->second->getPrimarySettlement();

	ASSERT_TRUE(primary.second);
	ASSERT_EQ(primary.second->getName(), "b_test");
}

TEST(CK2World_ProvincesTests, primarySettlementLinkCannotBeSetThrowsWarning)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "42=\n";
	input << "\t{primary_settlement=\"b_test\"\n";
	input << "\tb_test3={}\n";
	input << "\tb_test2={}\n";
	input << "\t}\n";
	input << "}\n";
	CK2::Provinces provinces(input);

	std::stringstream log;
	auto stdOutBuf = std::cout.rdbuf();
	std::cout.rdbuf(log.rdbuf());

	provinces.linkPrimarySettlements();

	std::cout.rdbuf(stdOutBuf);
	auto stringLog = log.str();
	auto newLine = stringLog.find_first_of('\n');
	stringLog = stringLog.substr(0, newLine);

	ASSERT_EQ(stringLog, "[WARNING] Primary barony ID: b_test is not in the province: 42 !");
}

TEST(CK2World_ProvincesTests, wonderCanBeLinked)
{
	std::stringstream input;
	input << "={42={}}\n";
	CK2::Provinces provinces(input);

	std::stringstream input2;
	input2 << "={7={name=\"theWonder\" province=42 active=yes stage=3}}\n";
	CK2::Wonders wonders(input2);

	provinces.linkWonders(wonders);

	const auto& provinceItr = provinces.getProvinces().find(42);
	const auto& wonder = provinceItr->second->getWonder();

	ASSERT_TRUE(wonder.second);
	ASSERT_EQ(wonder.second->getName(), "theWonder");
}

TEST(CK2World_ProvincesTests, multipleWondersCanBeLinked)
{
	std::stringstream input;
	input << "={\n";
	input << "43={}\n";
	input << "42={}\n";
	input << "44={}\n";
	input << "}\n";
	CK2::Provinces provinces(input);

	std::stringstream input2;
	input2 << "={\n";
	input2 << "8={name=\"theWonder\" province=42 active=yes stage=3}\n";
	input2 << "9={name=\"theWonder2\" province=44 active=yes stage=3}\n";
	input2 << "10={name=\"theWonder3\" province=43 active=yes stage=3}\n";
	input2 << "}\n";
	CK2::Wonders wonders(input2);

	provinces.linkWonders(wonders);

	const auto& provinceItr = provinces.getProvinces().find(42);
	const auto& wonder = provinceItr->second->getWonder();
	const auto& provinceItr2 = provinces.getProvinces().find(43);
	const auto& wonder2 = provinceItr2->second->getWonder();
	const auto& provinceItr3 = provinces.getProvinces().find(44);
	const auto& wonder3 = provinceItr3->second->getWonder();

	ASSERT_TRUE(wonder.second);
	ASSERT_EQ(wonder.second->getName(), "theWonder");
	ASSERT_TRUE(wonder2.second);
	ASSERT_EQ(wonder2.second->getName(), "theWonder3");
	ASSERT_TRUE(wonder3.second);
	ASSERT_EQ(wonder3.second->getName(), "theWonder2");
}

TEST(CK2World_ProvincesTests, inactiveWondersWillNotBeLinked)
{
	std::stringstream input;
	input << "={\n";
	input << "43={}\n";
	input << "42={}\n";
	input << "44={}\n";
	input << "}\n";
	CK2::Provinces provinces(input);

	std::stringstream input2;
	input2 << "={\n";
	input2 << "8={name=\"theWonder\" province=42 active=no stage=3}\n";	// inactive
	input2 << "9={name=\"theWonder2\" province=43 active=no stage=3}\n"; // inactive
	input2 << "10={name=\"theWonder3\" province=44 active=yes stage=3}\n";
	input2 << "}\n";
	CK2::Wonders wonders(input2);

	provinces.linkWonders(wonders);

	const auto& provinceItr = provinces.getProvinces().find(42);
	const auto& wonder = provinceItr->second->getWonder();
	const auto& provinceItr2 = provinces.getProvinces().find(43);
	const auto& wonder2 = provinceItr2->second->getWonder();
	const auto& provinceItr3 = provinces.getProvinces().find(44);
	const auto& wonder3 = provinceItr3->second->getWonder();

	ASSERT_FALSE(wonder.second);
	ASSERT_FALSE(wonder2.second);
	ASSERT_TRUE(wonder3.second);
	ASSERT_EQ(wonder3.second->getName(), "theWonder3");
}

TEST(CK2World_ProvincesTests, unfinishedWondersWillNotBeLinked)
{
	std::stringstream input;
	input << "={\n";
	input << "43={}\n";
	input << "42={}\n";
	input << "44={}\n";
	input << "}\n";
	CK2::Provinces provinces(input);

	std::stringstream input2;
	input2 << "={\n";
	input2 << "8={name=\"theWonder\" province=42 active=yes stage=2}\n";	 // unfinished
	input2 << "9={name=\"theWonder2\" province=43 active=yes stage=1}\n"; // unfinished
	input2 << "10={name=\"theWonder3\" province=44 active=yes stage=3}\n";
	input2 << "}\n";
	CK2::Wonders wonders(input2);

	provinces.linkWonders(wonders);

	const auto& provinceItr = provinces.getProvinces().find(42);
	const auto& wonder = provinceItr->second->getWonder();
	const auto& provinceItr2 = provinces.getProvinces().find(43);
	const auto& wonder2 = provinceItr2->second->getWonder();
	const auto& provinceItr3 = provinces.getProvinces().find(44);
	const auto& wonder3 = provinceItr3->second->getWonder();

	ASSERT_FALSE(wonder.second);
	ASSERT_FALSE(wonder2.second);
	ASSERT_TRUE(wonder3.second);
	ASSERT_EQ(wonder3.second->getName(), "theWonder3");
}

TEST(CK2World_ProvincesTests, BrokenLinkAttemptThrowsWarning)
{
	std::stringstream input;
	input << "={\n";
	input << "42={}\n";
	input << "44={}\n";
	input << "}\n";
	CK2::Provinces provinces(input);

	std::stringstream input2;
	input2 << "={\n";
	input2 << "8={name=\"theWonder\" province=42 active=yes stage=3}\n";
	input2 << "9={name=\"theWonder2\" province=43 active=yes stage=3}\n"; // No province 43.
	input2 << "}\n";
	CK2::Wonders wonders(input2);

	std::stringstream log;
	auto stdOutBuf = std::cout.rdbuf();
	std::cout.rdbuf(log.rdbuf());

	provinces.linkWonders(wonders);

	std::cout.rdbuf(stdOutBuf);
	auto stringLog = log.str();
	auto newLine = stringLog.find_first_of('\n');
	stringLog = stringLog.substr(0, newLine);

	ASSERT_EQ(stringLog, "[WARNING] Wonder 9 is in province 43 which doesn't exist?");
}
