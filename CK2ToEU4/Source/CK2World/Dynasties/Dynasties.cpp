#include "Dynasties.h"
#include "Log.h"
#include "ParserHelpers.h"
#include "Dynasty.h"

CK2::Dynasties::Dynasties(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void CK2::Dynasties::registerKeys()
{
	registerRegex("\\d+", [this](const std::string& theDynID, std::istream& theStream) {
		auto newDynasty = std::make_shared<Dynasty>(theStream, std::stoi(theDynID));
		dynasties.insert(std::pair(newDynasty->getID(), newDynasty));
		});
	registerRegex("[A-Za-z0-9\\_:.-]+", commonItems::ignoreItem);
}
