#include "Province.h"
#include "ParserHelpers.h"
#include "Log.h"
#include "Barony.h"
#include "../Titles/Title.h"

CK2::Province::Province(std::istream& theStream, int provID) : provinceID(provID)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void CK2::Province::registerKeys()
{
	registerKeyword("name", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString nameStr(theStream);
		name = nameStr.getString();
	});
	registerKeyword("culture", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString cultureStr(theStream);
		culture = cultureStr.getString();
	});
	registerKeyword("religion", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString religionStr(theStream);
		religion = religionStr.getString();
	});
	registerKeyword("primary_settlement", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString primarySettlementStr(theStream);
		primarySettlement = std::pair(primarySettlementStr.getString(), nullptr);
	});
	registerKeyword("max_settlements", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleInt maxSettInt(theStream);
		maxSettlements = maxSettInt.getInt();
	});
	registerRegex("b_[A-Za-z0-9_-]+", [this](const std::string& baronyName, std::istream& theStream) {
		auto barony = std::make_shared<Barony>(theStream, baronyName);
		baronies.insert(std::pair(baronyName, barony));
	});
	registerRegex("[A-Za-z0-9\\:_.-]+", commonItems::ignoreItem);
}

int CK2::Province::getBuildingWeight() const
{
	// Having a barony counts as 3. Every building level counts as +1.
	// As this translates to raw dev in province, ownership is not relevant.
	// TODO: Add trade posts and hospitals.
	
	int buildingWeight = 0;
	for (const auto& barony: baronies)
	{
		buildingWeight += 3 + barony.second->getBuildingCount();
	}
	return buildingWeight;
}
