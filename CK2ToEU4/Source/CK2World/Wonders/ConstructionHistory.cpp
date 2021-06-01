#include "ConstructionHistory.h"
#include "Log.h"
#include "ParserHelpers.h"
#include "CommonRegexes.h"

CK2::ConstructionHistory::ConstructionHistory(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void CK2::ConstructionHistory::registerKeys()
{
	registerKeyword("wonder_historical_event_character", [this](const std::string& mods, std::istream& theStream) {		
		builderID = commonItems::singleInt(theStream).getInt();
	});
	registerKeyword("wonder_historical_event_date", [this](const std::string& mods, std::istream& theStream) {		
		binaryDate = commonItems::singleInt(theStream).getInt();
	});
	registerKeyword("wonder_upgrade", [this](const std::string& mods, std::istream& theStream) {
		upgrade = commonItems::singleString(theStream).getString();
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}
