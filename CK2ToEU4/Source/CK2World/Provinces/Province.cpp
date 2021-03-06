#include "Province.h"
#include "../Titles/Title.h"
#include "Barony.h"
#include "CommonRegexes.h"
#include "Log.h"
#include "ParserHelpers.h"

CK2::Province::Province(std::istream& theStream, int provID): provinceID(provID)
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
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
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

std::optional<std::pair<std::string, std::shared_ptr<CK2::Title>>> CK2::Province::belongsToDuchy() const
{
	if (deJureTitle.second && deJureTitle.second->getLiege().second && deJureTitle.second->getLiege().second->getTitle().second &&
		 deJureTitle.second->getLiege().second->getTitle().first.find("d_") == 0)
		return deJureTitle.second->getLiege().second->getTitle();
	else
		return std::nullopt;
}

std::optional<std::pair<std::string, std::shared_ptr<CK2::Title>>> CK2::Province::belongsToKingdom() const
{
	if (deJureTitle.second && deJureTitle.second->getLiege().second && deJureTitle.second->getLiege().second->getTitle().second &&
		 deJureTitle.second->getLiege().second->getTitle().second->getLiege().second &&
		 deJureTitle.second->getLiege().second->getTitle().second->getLiege().second->getTitle().second &&
		 deJureTitle.second->getLiege().second->getTitle().second->getLiege().second->getTitle().first.find("k_") == 0)
		return deJureTitle.second->getLiege().second->getTitle().second->getLiege().second->getTitle();
	else
		return std::nullopt;
}