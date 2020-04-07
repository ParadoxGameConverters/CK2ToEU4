#ifndef EU4_PROVINCE_DETAILS_H
#define EU4_PROVINCE_DETAILS_H

#include "Color.h"
#include "ProvinceModifier.h"
#include "newParser.h"
#include <set>

namespace EU4
{
class ProvinceDetails: commonItems::parser
{
  public:
	ProvinceDetails() = default;
	explicit ProvinceDetails(const std::string& filePath);
	explicit ProvinceDetails(std::istream& theStream);

	// These values are open to ease management.
	// This is a storage container for EU4::Province.
	// Some of these values are loaded from files, others are adjusted on the fly.
	std::string owner;
	std::string controller;
	std::string capital; // output with quotes!
	bool isCity = false;
	std::string culture;
	std::string religion;
	bool inHre = false;
	int baseTax = 0;
	int baseProduction = 0;
	int baseManpower = 0;
	std::string tradeGoods;
	std::set<std::string> cores;
	std::set<std::string> discoveredBy;
	int extraCost = 0;
	int centerOfTrade = 0;
	int localAutonomy = 0;
	int nativeSize = 0;
	int nativeFerocity = 0;
	int nativeHostileness = 0;
	std::vector<ProvinceModifier> provinceModifiers;
	bool fort = false;
	std::string estate;
	std::set<std::string> latentGoods;
	bool shipyard = false;
	std::set<std::string> provinceTriggeredModifiers;
	int revoltRisk = 0;
	int unrest = 0;
	bool seatInParliament = false;
	bool jainsBurghers = false;
	bool vaisyasBurghers = false;
	bool rajputsNobles = false;
	bool brahminsChurch = false;
	int nationalism = 0;
	std::set<std::string> claims;

  private:
	void registerKeys();
};
} // namespace EU4

#endif // EU4_PROVINCE_DETAILS_H
