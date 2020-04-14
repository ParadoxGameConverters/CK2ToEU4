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
	registerKeyword("name", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString nameStr(theStream);
		name = nameStr.getString();
	});
	registerKeyword("stage", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleInt stageInt(theStream);
		stage = stageInt.getInt();
	});
	registerKeyword("province", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleInt provinceInt(theStream);
		provinceID = provinceInt.getInt();
	});
	registerKeyword("type", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString typeStr(theStream);
		type = typeStr.getString();
	});
	registerKeyword("active", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString activeStr(theStream);
		active = activeStr.getString() == "yes";
	});
	registerRegex("[A-Za-z0-9\\:_.-]+", commonItems::ignoreItem);
}
