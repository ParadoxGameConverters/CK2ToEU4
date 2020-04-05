#include "ProvinceMapping.h"
#include "ParserHelpers.h"

mappers::ProvinceMapping::ProvinceMapping(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::ProvinceMapping::registerKeys()
{
	registerKeyword("eu4", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleInt provinceInt(theStream);
		eu4Provinces.push_back(provinceInt.getInt());
		});
	registerKeyword("ck2", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleInt provinceInt(theStream);
		ck2Provinces.push_back(provinceInt.getInt());
		});
	registerRegex("[a-zA-Z0-9\\_.:-]+", commonItems::ignoreItem);
}
