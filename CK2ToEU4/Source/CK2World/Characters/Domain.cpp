#include "Domain.h"
#include "../Titles/Liege.h"
#include "Log.h"
#include "ParserHelpers.h"

CK2::Domain::Domain(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void CK2::Domain::registerKeys()
{
	registerKeyword("capital", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString capitalStr(theStream);
		capital = std::pair(capitalStr.getString(), nullptr);
	});
	registerKeyword("primary", [this](const std::string& unused, std::istream& theStream) {
		const auto primTitleStr = commonItems::singleItem(unused, theStream);
		if (primTitleStr.find('{') != std::string::npos)
		{
			std::stringstream tempStream(primTitleStr);
			auto newPrimTitle = std::make_shared<Liege>(tempStream);
			primaryTitle = std::pair(newPrimTitle->getTitle().first, newPrimTitle);
		}
		else
		{
			auto newPrimTitle = std::make_shared<Liege>(primTitleStr);
			primaryTitle = std::pair(newPrimTitle->getTitle().first, newPrimTitle);
		}
	});
	registerRegex("[A-Za-z0-9\\:_.-]+", commonItems::ignoreItem);
}
