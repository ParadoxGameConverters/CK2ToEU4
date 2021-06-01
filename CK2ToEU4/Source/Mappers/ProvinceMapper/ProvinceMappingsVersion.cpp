#include "ProvinceMappingsVersion.h"
#include "ParserHelpers.h"
#include "CommonRegexes.h"

mappers::ProvinceMappingsVersion::ProvinceMappingsVersion(std::istream& theStream)
{
	registerKeyword("link", [this](const std::string& unused, std::istream& theStream) {
		const ProvinceMapping newMapping(theStream);
		if (newMapping.getCK2Provinces().empty() && newMapping.getEU4Provinces().empty())
			return;
		mappings.push_back(newMapping);
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();
}