#include "GovernmentsMapping.h"
#include "Log.h"
#include "ParserHelpers.h"

mappers::GovernmentsMapping::GovernmentsMapping(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::GovernmentsMapping::registerKeys()
{
	registerKeyword("gov", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString govStr(theStream);
		government = govStr.getString();
	});
	registerKeyword("reform", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString reformStr(theStream);
		reform = reformStr.getString();
	});
	registerKeyword("ck2title", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString ck2titleStr(theStream);
		ck2title = ck2titleStr.getString();
	});
	registerKeyword("ck2gov", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString ck2govStr(theStream);
		ck2governments.insert(ck2govStr.getString());
	});
	registerRegex("[A-Za-z0-9\\_.:-]+", commonItems::ignoreItem);
}

bool mappers::GovernmentsMapping::matchGovernment(const std::string& ck2Government, const std::string& incTitle) const
{
	if (!ck2title.empty() && ck2title == incTitle) return true;
	if (ck2governments.count(ck2Government)) return true;
	return false;
}