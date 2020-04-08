#include "Liege.h"
#include "ParserHelpers.h"

CK2::Liege::Liege(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void CK2::Liege::registerKeys()
{
	registerKeyword("title", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString titleStr(theStream);
		title = std::make_pair(titleStr.getString(), nullptr);
	});
	registerKeyword("base_title", [this](const std::string& building, std::istream& theStream) {
		const commonItems::singleString baseTitleStr(theStream);
		baseTitle = std::make_pair(baseTitleStr.getString(), nullptr);
	});
	registerKeyword("is_custom", [this](const std::string& building, std::istream& theStream) {
		const commonItems::singleString customStr(theStream);
		custom = customStr.getString() == "yes";
	});
	registerKeyword("is_dynamic", [this](const std::string& building, std::istream& theStream) {
		const commonItems::singleString dynamicStr(theStream);
		dynamic = dynamicStr.getString() == "yes";
	});
	registerRegex("[A-Za-z0-9\\:_.-]+", commonItems::ignoreItem);
}
