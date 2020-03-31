#include "ID.h"
#include "ParserHelpers.h"
#include "Log.h"

ID::ID(std::istream& theStream)
{
	registerKeyword("id", [this](const std::string& unused, std::istream& theStream)
		{
			const commonItems::singleInt theNum(theStream);
			IDNum = theNum.getInt();
		}
	);
	registerKeyword("type", [this](const std::string& unused, std::istream& theStream)
		{
			const commonItems::singleInt theNum(theStream);
			IDType = theNum.getInt();
		}
	);
	registerRegex("[a-zA-Z0-9_]+", commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();
}