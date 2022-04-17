#include "CultureMapper.h"
#include "CommonRegexes.h"
#include "Log.h"
#include "ParserHelpers.h"

void mappers::CultureMapper::initCultureMapper(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::CultureMapper::initCultureMapper(const std::string& path)
{
	Log(LogLevel::Info) << "-> Parsing culture mappings.";
	registerKeys();
	std::string dirPath = "configurables";
	if (!path.empty())
	{
		Log(LogLevel::Info) << "Culture Mapper override: " << path;
		dirPath += "/" + path;
	}
	parseFile(dirPath + "/culture_map.txt");
	clearRegisteredKeywords();
	Log(LogLevel::Info) << "<> Loaded " << cultureMapRules.size() << " cultural links.";
}

void mappers::CultureMapper::loadRegionMapper(std::shared_ptr<RegionMapper> theRegionMapper)
{
	for (auto& rule: cultureMapRules)
		rule.insertRegionMapper(theRegionMapper);
}

void mappers::CultureMapper::registerKeys()
{
	registerKeyword("link", [this](const std::string& unused, std::istream& theStream) {
		const CultureMappingRule rule(theStream);
		cultureMapRules.push_back(rule);
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}

std::optional<std::string> mappers::CultureMapper::cultureMatch(const std::string& ck2culture,
	 const std::string& eu4religion,
	 int eu4Province,
	 const std::string& eu4ownerTag) const
{
	for (const auto& cultureMappingRule: cultureMapRules)
	{
		const auto& possibleMatch = cultureMappingRule.cultureMatch(ck2culture, eu4religion, eu4Province, eu4ownerTag);
		if (possibleMatch)
			return *possibleMatch;
	}
	return std::nullopt;
}

std::optional<std::string> mappers::CultureMapper::cultureRegionalMatch(const std::string& ck2culture,
	 const std::string& eu4religion,
	 int eu4Province,
	 const std::string& eu4ownerTag) const
{
	for (const auto& cultureMappingRule: cultureMapRules)
	{
		const auto& possibleMatch = cultureMappingRule.cultureRegionalMatch(ck2culture, eu4religion, eu4Province, eu4ownerTag);
		if (possibleMatch)
			return *possibleMatch;
	}
	return std::nullopt;
}

std::optional<std::string> mappers::CultureMapper::cultureNonRegionalNonReligiousMatch(const std::string& ck2culture,
	 const std::string& eu4religion,
	 int eu4Province,
	 const std::string& eu4ownerTag) const
{
	for (const auto& cultureMappingRule: cultureMapRules)
	{
		const auto& possibleMatch = cultureMappingRule.cultureNonRegionalNonReligiousMatch(ck2culture, eu4religion, eu4Province, eu4ownerTag);
		if (possibleMatch)
			return *possibleMatch;
	}
	return std::nullopt;
}

std::optional<std::string> mappers::CultureMapper::getTechGroup(const std::string& incEU4Culture) const
{
	for (const auto& mapping: cultureMapRules)
		if (mapping.getTechGroup(incEU4Culture))
			return mapping.getTechGroup(incEU4Culture);
	return std::nullopt;
}

std::optional<std::string> mappers::CultureMapper::getGFX(const std::string& incEU4Culture) const
{
	for (const auto& mapping: cultureMapRules)
		if (mapping.getGFX(incEU4Culture))
			return mapping.getGFX(incEU4Culture);
	return std::nullopt;
}