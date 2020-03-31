#include "Provinces.h"
#include "Log.h"
#include "ParserHelpers.h"

CK2::Provinces::Provinces(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void CK2::Provinces::registerKeys()
{
	registerRegex("\\d+", [this](const std::string& provID, std::istream& theStream) {
		auto newProvince = Province(theStream, std::stoi(provID));
		provinces.insert(std::pair(newProvince.getID(), newProvince));
		});
	registerRegex("[A-Za-z0-9\\_:.-]+", commonItems::ignoreItem);
}
