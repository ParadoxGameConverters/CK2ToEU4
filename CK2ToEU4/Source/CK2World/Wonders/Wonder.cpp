#include "Wonder.h"
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
			fillConstructionHistory(tempStream);
		}
	});
	registerKeyword("stage", [this](const std::string& unused, std::istream& theStream) {
		stage = commonItems::singleInt(theStream).getInt();
	});
	registerKeyword("active", [this](const std::string& unused, std::istream& theStream) {
		active = (commonItems::singleString(theStream).getString() == "yes");
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}
void CK2::Wonder::fillConstructionHistory(std::istream& theStream)
{
	registerKeyword("wonder_historical_event_character", [this](const std::string& mods, std::istream& theStream) {
		const auto& tempBuilder = commonItems::singleLlong(theStream).getLlong();
		if (builder == 0)
			builder = tempBuilder;
			
	});
	registerKeyword("wonder_historical_event_date", [this](const std::string& mods, std::istream& theStream) {
		const auto& tempDate = commonItems::singleInt(theStream).getInt();
		if (binaryDate < 1 || tempDate < binaryDate) // Gets the earliest date
			binaryDate = tempDate;
	});
	registerKeyword("wonder_upgrade", [this](const std::string& mods, std::istream& theStream) {
		upgrades.insert(commonItems::singleString(theStream).getString());
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}

void CK2::Wonder::setTrueDate(int binaryDate)
{
	short hours = binaryDate % 24;
	binaryDate /= 24;
	short month_day_from_julian = binaryDate % 365;
	binaryDate /= 365;
	binaryDate -= 5000;

	short month = 12;
	short days = 1;
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

	trueDate = (binaryDate + "." + std::to_string(month) + "." + std::to_string(days));
}