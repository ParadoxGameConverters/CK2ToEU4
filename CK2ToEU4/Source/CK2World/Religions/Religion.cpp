#include "Religion.h"
#include "ParserHelpers.h"


CK2::Religion::Religion(std::istream& theStream, std::string theFeatures): features(std::move(theFeatures))
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void CK2::Religion::registerKeys()
{
	registerKeyword("features", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString typeStr(theStream);
		features = typeStr.getString();
	});
	registerRegex("[A-Za-z0-9\\:_.-]+", commonItems::ignoreItem);
}