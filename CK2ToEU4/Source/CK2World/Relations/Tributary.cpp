#include "Tributary.h"
#include "ParserHelpers.h"

CK2::Tributary::Tributary(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void CK2::Tributary::registerKeys()
{
	registerKeyword("tributary_type", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString typeStr(theStream);
		tributaryType = typeStr.getString();
	});
	registerKeyword("tributary", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleInt tributaryInt(theStream);
		tributaryID = tributaryInt.getInt();
	});
	registerRegex("[A-Za-z0-9\\:_.-]+", commonItems::ignoreItem);
}
