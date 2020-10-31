#include "Offmap.h"
#include "Log.h"
#include "ParserHelpers.h"

CK2::Offmap::Offmap(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void CK2::Offmap::registerKeys()
{
	registerKeyword("names", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::stringList nameStrList(theStream);
		const auto& theList = nameStrList.getStrings();
		name = theList.back(); // The last name is the actual china name.
	});
	registerKeyword("type", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString typeStr(theStream);
		type = typeStr.getString();
	});
	registerKeyword("holder", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleInt holderInt(theStream);
		holder = std::pair(holderInt.getInt(), nullptr);
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}
