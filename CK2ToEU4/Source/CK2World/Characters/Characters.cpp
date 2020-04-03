#include "Characters.h"
#include "Log.h"
#include "ParserHelpers.h"
#include "Character.h"

CK2::Characters::Characters(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void CK2::Characters::registerKeys()
{
	registerRegex("\\d+", [this](const std::string& charID, std::istream& theStream) {
		auto newCharacter = std::make_shared<Character>(theStream, std::stoi(charID));
		characters.insert(std::pair(newCharacter->getID(), newCharacter));
		});
	registerRegex("[A-Za-z0-9\\_:.-]+", commonItems::ignoreItem);
}
