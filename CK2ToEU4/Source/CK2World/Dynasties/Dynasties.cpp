#include "Dynasties.h"
#include "CommonRegexes.h"
#include "Dynasty.h"
#include "ParserHelpers.h"

CK2::Dynasties::Dynasties(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void CK2::Dynasties::loadDynasties(const std::filesystem::path& thePath)
{
	registerKeys();
	parseFile(thePath);
	clearRegisteredKeywords();
}

void CK2::Dynasties::underLoadDynasties(const std::filesystem::path& thePath)
{
	registerUnderKeys();
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
		if (dynasties.count(std::stoi(theDynID)))
		{
			dynasties[std::stoi(theDynID)]->updateDynasty(theStream);
		}
		else
		{
			auto newDynasty = std::make_shared<Dynasty>(theStream, std::stoi(theDynID));
			dynasties.insert(std::pair(newDynasty->getID(), newDynasty));
		}
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}

void CK2::Dynasties::registerUnderKeys()
{
	registerRegex("\\d+", [this](const std::string& theDynID, std::istream& theStream) {
		if (dynasties.count(std::stoi(theDynID)))
		{
			dynasties[std::stoi(theDynID)]->underUpdateDynasty(theStream);
		}
		else
		{
			auto newDynasty = std::make_shared<Dynasty>(theStream, std::stoi(theDynID));
			dynasties.insert(std::pair(newDynasty->getID(), newDynasty));
		}
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}
