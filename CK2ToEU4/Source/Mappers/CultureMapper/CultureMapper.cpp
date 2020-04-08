#include "CultureMapper.h"
#include "Log.h"
#include "ParserHelpers.h"

mappers::CultureMapper::CultureMapper(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

mappers::CultureMapper::CultureMapper()
{
	LOG(LogLevel::Info) << "-> Parsing culture mappings.";
	registerKeys();
	parseFile("configurables/culture_map.txt");
	clearRegisteredKeywords();
	LOG(LogLevel::Info) << "<> Loaded " << cultureMapRules.size() << " cultural links.";
}

void mappers::CultureMapper::loadRegionMapper(std::shared_ptr<RegionMapper> theRegionMapper)
{
	for (auto& rule: cultureMapRules) rule.insertRegionMapper(theRegionMapper);
}

void mappers::CultureMapper::registerKeys()
{
	registerKeyword("link", [this](const std::string& unused, std::istream& theStream) {
		const CultureMappingRule rule(theStream);
		cultureMapRules.push_back(rule);
	});
	registerRegex("[a-zA-Z0-9\\_.:]+", commonItems::ignoreItem);
}

std::optional<std::string> mappers::CultureMapper::cultureMatch(const std::string& eu4culture,
	 const std::string& eu4religion,
	 int eu4Province,
	 const std::string& eu4ownerTag) const
{
	for (const auto& cultureMappingRule: cultureMapRules) {
		const auto& possibleMatch = cultureMappingRule.cultureMatch(eu4culture, eu4religion, eu4Province, eu4ownerTag);
		if (possibleMatch) return *possibleMatch;
	}
	return std::nullopt;
}

std::optional<std::string> mappers::CultureMapper::cultureRegionalMatch(const std::string& eu4culture,
	 const std::string& eu4religion,
	 int eu4Province,
	 const std::string& eu4ownerTag) const
{
	for (const auto& cultureMappingRule: cultureMapRules) {
		const auto& possibleMatch = cultureMappingRule.cultureRegionalMatch(eu4culture, eu4religion, eu4Province, eu4ownerTag);
		if (possibleMatch) return *possibleMatch;
	}
	return std::nullopt;
}

std::optional<std::string> mappers::CultureMapper::cultureNonRegionalNonReligiousMatch(const std::string& eu4culture,
	 const std::string& eu4religion,
	 int eu4Province,
	 const std::string& eu4ownerTag) const
{
	for (const auto& cultureMappingRule: cultureMapRules) {
		const auto& possibleMatch = cultureMappingRule.cultureNonRegionalNonReligiousMatch(eu4culture, eu4religion, eu4Province, eu4ownerTag);
		if (possibleMatch) return *possibleMatch;
	}
	return std::nullopt;
}
