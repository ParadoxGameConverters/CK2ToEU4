#include "GovernmentsMapper.h"
#include "CommonRegexes.h"
#include "Log.h"
#include "ParserHelpers.h"

void mappers::GovernmentsMapper::initGovernmentsMapper(const std::filesystem::path& path)
{
	Log(LogLevel::Info) << "-> Parsing government mappings.";
	registerKeys();
	std::filesystem::path dirPath = "configurables";
	if (!path.empty())
	{
		Log(LogLevel::Info) << "Governments Mapper override: " << path.string();
		dirPath = dirPath / path;
	}
	parseFile(dirPath / "culture_map.txt");
	parseFile(dirPath / "government_map.txt");
	clearRegisteredKeywords();
	Log(LogLevel::Info) << "<> Loaded " << govMappings.size() << " governmental links.";
}

void mappers::GovernmentsMapper::initGovernmentsMapper(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::GovernmentsMapper::registerKeys()
{
	registerKeyword("link", [this](const std::string& unused, std::istream& theStream) {
		const GovernmentsMapping newMapping(theStream);
		govMappings.emplace_back(newMapping);
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}

std::optional<std::pair<std::string, std::string>> mappers::GovernmentsMapper::matchGovernment(const std::string& ck2Government,
	 const std::string& ck2Title) const
{
	std::pair<std::string, std::string> toReturn;

	// first iterate over those that have a ck2title field, they take priority.
	for (const auto& mapping: govMappings)
	{
		if (mapping.getCK2Title().empty())
			continue;
		if (mapping.matchGovernment(ck2Government, ck2Title))
		{
			toReturn.first = mapping.getGovernment();
			toReturn.second = mapping.getReform();
			return toReturn;
		}
	}

	// Then might as well retry.
	for (const auto& mapping: govMappings)
		if (mapping.matchGovernment(ck2Government, ck2Title))
		{
			toReturn.first = mapping.getGovernment();
			toReturn.second = mapping.getReform();
			return toReturn;
		}
	return std::nullopt;
}
