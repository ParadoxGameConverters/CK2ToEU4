#include "Wonder.h"
#include "CommonRegexes.h"
#include "ConstructionHistory.h"
#include "Log.h"
#include "ParserHelpers.h"

CK2::Wonder::Wonder(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void CK2::Wonder::registerKeys()
{
	registerKeyword("type", [this](const std::string& unused, std::istream& theStream) {
		type = commonItems::singleString(theStream).getString();
		upgrades.insert(type);
	});
	registerKeyword("province", [this](const std::string& unused, std::istream& theStream) {
		provinceID = commonItems::singleInt(theStream).getInt();
	});
	registerKeyword("name", [this](const std::string& unused, std::istream& theStream) {
		name = commonItems::singleString(theStream).getString();
	});
	registerKeyword("desc", [this](const std::string& mods, std::istream& theStream) {
		desc = commonItems::singleString(theStream).getString();
	});
	registerKeyword("construction_history", [this](const std::string& mods, std::istream& theStream) {
		for (const auto& blob: commonItems::blobList(theStream).getBlobs())
		{
			std::stringstream tempStream(blob);
			auto historyItem = ConstructionHistory(tempStream);
			if (const auto& tempBuilderID = historyItem.getBuilderID(); builderID == 0 && tempBuilderID > 0)
				builderID = tempBuilderID;
			if (const auto& tempDate = historyItem.getBinaryDate(); binaryDate == 0 || tempDate < binaryDate) // Gets the earliest date
				binaryDate = tempDate;
			if (!historyItem.getUpgrade().empty())
				upgrades.insert(historyItem.getUpgrade());
		}
	});
	registerKeyword("stage", [this](const std::string& unused, std::istream& theStream) {
		stage = commonItems::singleInt(theStream).getInt();
		if (stage < 0)
		{
			stage = 0;
			upgrades.emplace("generic_misc_upgrade_3");
		}
		else if (stage > 3) // For mods
		{
			stage = 3;
		}
	});
	registerKeyword("active", [this](const std::string& unused, std::istream& theStream) {
		active = (commonItems::singleString(theStream).getString() == "yes");
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}

void CK2::Wonder::setTrueDate(int binDate)
{
	binDate /= 24;
	const auto month_day_from_julian = binDate % 365;
	binDate /= 365;
	binDate -= 5000;

	auto month = 12;
	int days;
	if (month_day_from_julian < 30)
	{
		month = 1;
		days = month_day_from_julian + 1;
	}
	else if (month_day_from_julian < 58)
	{
		month = 2;
		days = month_day_from_julian - 30;
	}
	else if (month_day_from_julian < 89)
	{
		month = 3;
		days = month_day_from_julian - 58;
	}
	else if (month_day_from_julian < 119)
	{
		month = 4;
		days = month_day_from_julian - 89;
	}
	else if (month_day_from_julian < 150)
	{
		month = 5;
		days = month_day_from_julian - 119;
	}
	else if (month_day_from_julian < 180)
	{
		month = 6;
		days = month_day_from_julian - 150;
	}
	else if (month_day_from_julian < 211)
	{
		month = 7;
		days = month_day_from_julian - 180;
	}
	else if (month_day_from_julian < 242)
	{
		month = 8;
		days = month_day_from_julian - 211;
	}
	else if (month_day_from_julian < 272)
	{
		month = 9;
		days = month_day_from_julian - 242;
	}
	else if (month_day_from_julian < 303)
	{
		month = 10;
		days = month_day_from_julian - 272;
	}
	else if (month_day_from_julian < 333)
	{
		month = 11;
		days = month_day_from_julian - 303;
	}
	else
		days = month_day_from_julian - 333;

	trueDate = std::to_string(binDate) + "." + std::to_string(month) + "." + std::to_string(days);
}