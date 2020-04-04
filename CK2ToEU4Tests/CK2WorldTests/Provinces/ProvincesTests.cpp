#include "gtest/gtest.h"
#include <sstream>
#include "../../CK2ToEU4/Source/CK2World/Provinces/Provinces.h"
#include "../../CK2ToEU4/Source/CK2World/Provinces/Province.h"
#include "../../CK2ToEU4/Source/CK2World/Provinces/Barony.h"
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
	auto newLine = stringLog.find_first_of("\n");
	stringLog = stringLog.substr(0, newLine);

	ASSERT_EQ(stringLog, "Primary barony ID: b_test is not in the province: 42 !");
}

