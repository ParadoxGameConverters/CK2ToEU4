#include "Domain.h"
#include "ParserHelpers.h"
#include "Log.h"

CK2::Domain::Domain(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void CK2::Domain::registerKeys()
{
	registerKeyword("capital", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString capitalStr(theStream);
		capital = capitalStr.getString();
		});
	registerKeyword("primary", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString primaryStr(theStream);
		primaryTitle = primaryStr.getString();
		});
	registerRegex("[A-Za-z0-9\\:_.-]+", commonItems::ignoreItem);
}
