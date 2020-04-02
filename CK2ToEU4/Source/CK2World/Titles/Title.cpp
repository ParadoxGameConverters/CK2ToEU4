#include "Title.h"
#include "ParserHelpers.h"
#include "Log.h"

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
		holder = holderInt.getInt();
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
			liege = Liege(tempStream);			
		}
		else
		{
			liege = Liege(liegeStr);
		}
		});
	registerKeyword("de_jure_liege", [this](const std::string& unused, std::istream& theStream) {
		// This can again either be a single string or a Liege object.
		const auto djLiegeStr = commonItems::singleItem(unused, theStream);
		if (djLiegeStr.find("{") != std::string::npos)
		{
			std::stringstream tempStream(djLiegeStr);
			deJureLiege = Liege(tempStream);			
		}
		else
		{
			deJureLiege = Liege(djLiegeStr);
		}
		});
	registerRegex("[A-Za-z0-9\\:_.-]+", commonItems::ignoreItem);
}
