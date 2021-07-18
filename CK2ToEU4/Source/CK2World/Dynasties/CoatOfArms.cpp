#include "CoatOfArms.h"
#include "CommonRegexes.h"
#include "Log.h"
#include "ParserHelpers.h"

CK2::CoatOfArms::CoatOfArms(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void CK2::CoatOfArms::registerKeys()
{
	registerKeyword("religion", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString religionStr(theStream);
		religion = religionStr.getString();
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}
