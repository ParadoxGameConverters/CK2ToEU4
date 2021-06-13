#include "Wonders.h"
#include "CommonRegexes.h"
#include "Log.h"
#include "ParserHelpers.h"
#include "Wonder.h"

CK2::Wonders::Wonders(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void CK2::Wonders::registerKeys()
{
	registerRegex(R"(\d+)", [this](const std::string& wonderID, std::istream& theStream) {
		auto newWonder = std::make_shared<Wonder>(theStream);
		newWonder->setWonderID(std::stoi(wonderID));
		wonders.insert(std::pair(std::stoi(wonderID), newWonder));
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}
