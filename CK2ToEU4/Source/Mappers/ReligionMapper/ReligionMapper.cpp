#include "ReligionMapper.h"
#include "Log.h"
#include "ParserHelpers.h"
#include "ReligionMapping.h"

mappers::ReligionMapper::ReligionMapper()
{
	LOG(LogLevel::Info) << "-> Parsing religion mappings.";
	registerKeys();
	parseFile("configurables/religion_map.txt");
	clearRegisteredKeywords();
	LOG(LogLevel::Info) << "<> Loaded " << ck2ToEu4ReligionMap.size() << " religious links.";
}

mappers::ReligionMapper::ReligionMapper(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::ReligionMapper::registerKeys()
{
	registerKeyword("link", [this](const std::string& unused, std::istream& theStream) {
		const ReligionMapping theMapping(theStream);
		for (const auto& ck2Religion: theMapping.getCK2Religions())
		{
			ck2ToEu4ReligionMap.insert(std::make_pair(ck2Religion, theMapping.getEU4Religion()));
		}
	});
	registerRegex("[a-zA-Z0-9\\_.:]+", commonItems::ignoreItem);
}

std::optional<std::string> mappers::ReligionMapper::getEu4ReligionForCk2Religion(const std::string& ck2Religion) const
{
	const auto& mapping = ck2ToEu4ReligionMap.find(ck2Religion);
	if (mapping != ck2ToEu4ReligionMap.end())
		return mapping->second;
	return std::nullopt;
}