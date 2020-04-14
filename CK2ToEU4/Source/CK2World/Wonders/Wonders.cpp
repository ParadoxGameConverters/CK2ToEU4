#include "Wonders.h"
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
	registerRegex("\\d+", [this](const std::string& wonderID, std::istream& theStream) {
		auto newWonder = std::make_shared<Wonder>(theStream);
		wonders.insert(std::pair(std::stoi(wonderID), newWonder));
	});
	registerRegex("[A-Za-z0-9\\_:.-]+", commonItems::ignoreItem);
}
