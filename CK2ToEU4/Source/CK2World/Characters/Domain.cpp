#include "Domain.h"
#include "ParserHelpers.h"
#include "Log.h"
#include "../Provinces/Barony.h"
#include "../Titles/Title.h"

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
		capital = std::pair(capitalStr.getString(), nullptr);
		});
	registerKeyword("primary", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString primaryStr(theStream);
		primaryTitle = std::pair(primaryStr.getString(), nullptr);
		});
	registerRegex("[A-Za-z0-9\\:_.-]+", commonItems::ignoreItem);
}
