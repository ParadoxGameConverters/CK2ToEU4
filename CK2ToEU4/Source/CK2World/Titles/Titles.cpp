#include "Titles.h"
#include "Log.h"
#include "ParserHelpers.h"

CK2::Titles::Titles(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void CK2::Titles::registerKeys()
{
	registerRegex("[A-Za-z0-9-_]+", [this](const std::string& titleName, std::istream& theStream) {
		auto newTitle = Title(theStream, titleName);
		titles.insert(std::pair(newTitle.getName(), newTitle));
		});
	registerRegex("[A-Za-z0-9\\_:.-]+", commonItems::ignoreItem);
}
