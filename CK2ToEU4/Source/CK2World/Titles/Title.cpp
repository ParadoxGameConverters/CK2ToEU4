#include "Title.h"
#include "ParserHelpers.h"
#include "Log.h"
#include "Liege.h"
#include "../Characters/Character.h"

CK2::Title::Title(std::istream& theStream, std::string theName) : name(std::move(theName))
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void CK2::Title::registerKeys()
{
	registerKeyword("holder", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleInt holderInt(theStream);
		holder = std::pair(holderInt.getInt(), nullptr);
		});
	registerKeyword("law", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString lawStr(theStream);
		laws.insert(lawStr.getString());
		});
	registerKeyword("liege", [this](const std::string& unused, std::istream& theStream) {
		// This can either be a single string or a Liege object.
		const auto liegeStr = commonItems::singleItem(unused, theStream);
		if (liegeStr.find("{") != std::string::npos)
		{
			std::stringstream tempStream(liegeStr);
			auto newLiege = std::make_shared<Liege>(tempStream);
			liege = std::pair(newLiege->getTitle().first, newLiege);
		}
		else
		{
			auto newLiege = std::make_shared<Liege>(liegeStr);
			liege = std::pair(newLiege->getTitle().first, newLiege);
		}
		});
	registerKeyword("de_jure_liege", [this](const std::string& unused, std::istream& theStream) {
		// This can again either be a single string or a Liege object.
		const auto djLiegeStr = commonItems::singleItem(unused, theStream);
		if (djLiegeStr.find("{") != std::string::npos)
		{
			std::stringstream tempStream(djLiegeStr);
			auto newdjLiege = std::make_shared<Liege>(tempStream);
			deJureLiege = std::pair(newdjLiege->getTitle().first, newdjLiege);
		}
		else
		{
			auto newdjLiege = std::make_shared<Liege>(djLiegeStr);
			deJureLiege = std::pair(newdjLiege->getTitle().first, newdjLiege);
		}
		});
	registerRegex("[A-Za-z0-9\\:_.-]+", commonItems::ignoreItem);
}
