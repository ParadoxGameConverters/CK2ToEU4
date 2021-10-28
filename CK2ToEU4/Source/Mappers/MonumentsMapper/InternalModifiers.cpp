#include "InternalModifiers.h"
#include "CommonRegexes.h"
#include "ParserHelpers.h"
#include <iomanip>

mappers::InternalModifiers::InternalModifiers(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::InternalModifiers::registerKeys()
{
	registerRegex(commonItems::catchallRegex, [this](const std::string& mod, std::istream& theStream) {
		modifierMap.emplace(mod, commonItems::getDoubles(theStream));
	});
}