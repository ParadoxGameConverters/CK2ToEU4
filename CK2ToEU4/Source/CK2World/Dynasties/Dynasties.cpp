#include "Dynasties.h"
#include "Dynasty.h"
#include "Log.h"
#include "ParserHelpers.h"

CK2::Dynasties::Dynasties(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void CK2::Dynasties::loadDynasties(const std::string& thePath)
{
	registerKeys();
	parseFile(thePath);
	clearRegisteredKeywords();
}

void CK2::Dynasties::loadDynasties(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void CK2::Dynasties::registerKeys()
{
	registerRegex("\\d+", [this](const std::string& theDynID, std::istream& theStream) {
		if (dynasties.count(std::stoi(theDynID))) {
			dynasties[std::stoi(theDynID)]->updateDynasty(theStream);
		} else {
			auto newDynasty = std::make_shared<Dynasty>(theStream, std::stoi(theDynID));
			dynasties.insert(std::pair(newDynasty->getID(), newDynasty));
		}
	});
	registerRegex("[A-Za-z0-9\\_:.-]+", commonItems::ignoreItem);
}
