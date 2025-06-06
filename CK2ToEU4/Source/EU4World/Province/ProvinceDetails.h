#ifndef EU4_PROVINCE_DETAILS_H
#define EU4_PROVINCE_DETAILS_H

#include "Parser.h"
#include "ProvinceModifier.h"
#include <set>

namespace EU4
{
class ProvinceDetails: commonItems::parser
{
  public:
	ProvinceDetails() = default;
	explicit ProvinceDetails(const std::filesystem::path& filePath);
	explicit ProvinceDetails(std::istream& theStream);
	void updateWith(const std::filesystem::path& filePath);

	// These values are open to ease management.
	// This is a storage container for EU4::Province.
	// Some of these values are loaded from files, others are adjusted on the fly.
	bool isCity = false;
	bool inHre = false;
	bool fort = false;
	bool shipyard = false;
	bool seatInParliament = false;
	bool jainsBurghers = false;
	bool vaisyasBurghers = false;
	bool rajputsNobles = false;
	bool brahminsChurch = false;
	int nationalism = 0;
	int revoltRisk = 0;
	int unrest = 0;
	int baseTax = 0;
	int baseProduction = 0;
	int baseManpower = 0;
	int extraCost = 0;
	int centerOfTrade = 0;
	int localAutonomy = 0;
	int nativeSize = 0;
	int nativeFerocity = 0;
	int nativeHostileness = 0;
	std::string owner;
	std::string controller;
	std::string capital; // output with quotes!
	std::string culture;
	std::string religion;
	std::string tradeGoods;
	std::string estate;
	std::string datedInfo; // For things set with 1444.1.1
	std::set<std::string> cores;
	std::set<std::string> discoveredBy;
	std::set<std::string> latentGoods;
	std::set<std::string> provinceTriggeredModifiers;
	std::set<std::string> claims;
	std::set<std::string> permanentClaims;
	std::vector<ProvinceModifier> provinceModifiers;

  private:
	void registerKeys();
};
} // namespace EU4

#endif // EU4_PROVINCE_DETAILS_H
