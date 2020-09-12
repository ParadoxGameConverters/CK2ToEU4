#include "Religion.h"
#include "ParserHelpers.h"


CK2::Religion::Religion(std::istream& theStream, const std::string& theFeatures)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void CK2::Religion::registerKeys()
{
	registerKeyword("features", [this](const std::string& unused, std::istream& theStream) {
		commonItems::stringList featureList(theStream);
		auto featureVector = featureList.getStrings();
		features = std::vector(featureVector.begin(), featureVector.end());
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}