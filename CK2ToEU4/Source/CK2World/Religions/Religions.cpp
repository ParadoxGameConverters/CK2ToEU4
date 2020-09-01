#include "Religion.h"
#include "Religions.h"
#include "ParserHelpers.h"


CK2::Religions::Religions(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void CK2::Religions::registerKeys()
{
	registerRegex(commonItems::catchallRegex, [this](const std::string& religionName, std::istream& theStream) {
		auto newReligion = std::make_shared<Religion>(theStream, religionName);
		reformedReligions.insert(std::pair(religionName, newReligion->getFeatures()));
		
	});
	registerRegex("[A-Za-z0-9\\_:.-]+", commonItems::ignoreItem);
}
