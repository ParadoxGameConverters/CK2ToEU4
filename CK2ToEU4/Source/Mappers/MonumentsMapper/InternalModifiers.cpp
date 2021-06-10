#include "Log.h"
#include "ParserHelpers.h"
#include "InternalModifiers.h"
#include <iomanip>
#include <set>
#include "CommonRegexes.h"

mappers::InternalModifiers::InternalModifiers(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::InternalModifiers::registerKeys()
{
	registerRegex(commonItems::catchallRegex, [this](const std::string& mod, std::istream& theStream) {
		modifierType = mod;
		modifierValues = commonItems::doubleList(theStream).getDoubles();	
	});
}