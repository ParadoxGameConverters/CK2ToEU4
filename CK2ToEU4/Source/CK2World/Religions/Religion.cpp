#include "Religion.h"
#include "ParserHelpers.h"


CK2::Religion::Religion(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void CK2::Religion::registerKeys()
{
	registerKeyword("features", [this](const std::string& unused, std::istream& theStream) {
		const auto featureVector = commonItems::stringList(theStream).getStrings();
		features = std::vector(featureVector.begin(), featureVector.end());
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}