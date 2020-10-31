#include "Relation.h"
#include "ParserHelpers.h"

CK2::Relation::Relation(std::istream& theStream, const int second): secondCharacterID(second)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void CK2::Relation::registerKeys()
{
	registerKeyword("tributary", [this](const std::string& unused, std::istream& theStream) {
		tributary = Tributary(theStream);
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}
