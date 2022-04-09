#include "ReligionMapper.h"
#include "CommonRegexes.h"
#include "Log.h"
#include "ParserHelpers.h"
#include "ReligionMapping.h"

void mappers::ReligionMapper::initReligionMapper(const std::string& path)
{
	Log(LogLevel::Info) << "-> Parsing religion mappings.";
	registerKeys();
	std::string dirPath = "configurables";
	if (!path.empty())
	{
		Log(LogLevel::Info) << "Religion Mapper override: " << path;
		dirPath += "/" + path;
	}
	parseFile(dirPath + "/religion_map.txt");
	clearRegisteredKeywords();
	Log(LogLevel::Info) << "<> Loaded " << ck2ToEu4ReligionMap.size() << " religious links.";
}

void mappers::ReligionMapper::initReligionMapper(std::istream& theStream)
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
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}

std::optional<std::string> mappers::ReligionMapper::getEu4ReligionForCk2Religion(const std::string& ck2Religion) const
{
	const auto& mapping = ck2ToEu4ReligionMap.find(ck2Religion);
	if (mapping != ck2ToEu4ReligionMap.end())
		return mapping->second;
	return std::nullopt;
}