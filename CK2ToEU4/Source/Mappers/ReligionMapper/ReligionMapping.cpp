#include "ReligionMapping.h"
#include "ParserHelpers.h"

mappers::ReligionMapping::ReligionMapping(std::istream& theStream)
{
	registerKeyword("eu4", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString religionString(theStream);
		eu4Religion = religionString.getString();
	});
	registerKeyword("ck2", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString religionString(theStream);
		ck2Religions.insert(religionString.getString());
	});
	registerRegex("[a-zA-Z0-9\\_.:]+", commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();
}