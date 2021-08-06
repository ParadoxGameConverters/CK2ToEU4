#include "TitleTagMapper.h"
#include "CommonRegexes.h"
#include "Log.h"
#include "ParserHelpers.h"
#include <iomanip>

mappers::TitleTagMapper::TitleTagMapper()
{
	Log(LogLevel::Info) << "-> Parsing Tag mappings";
	registerKeys();
	parseFile("configurables/tag_mappings.txt");
	clearRegisteredKeywords();
	registerChineseKeys();
	parseFile("configurables/chinese_tag_mappings.txt");
	clearRegisteredKeywords();
	Log(LogLevel::Info) << "<> " << theMappings.size() << " mappings and " << chineseMappings.size() << " Chinas loaded.";
}

mappers::TitleTagMapper::TitleTagMapper(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

mappers::TitleTagMapper::TitleTagMapper(std::istream& theStream, std::istream& chineseStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
	registerChineseKeys();
	parseStream(chineseStream);
	clearRegisteredKeywords();
}

void mappers::TitleTagMapper::registerKeys()
{
	registerKeyword("link", [this](const std::string& unused, std::istream& theStream) {
		theMappings.emplace_back(TitleTagMapping(theStream));
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}

void mappers::TitleTagMapper::registerChineseKeys()
{
	registerKeyword("link", [this](const std::string& unused, std::istream& theStream) {
		chineseMappings.emplace_back(TitleTagMapping(theStream));
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}

void mappers::TitleTagMapper::registerTitle(const std::string& ck2title, const std::string& eu4tag)
{
	registeredTitleTags.insert(std::pair(ck2title, eu4tag));
	usedTags.insert(eu4tag);
}

std::optional<std::string> mappers::TitleTagMapper::getTagForTitle(const std::string& ck2Title)
{
	return getTagForTitle(ck2Title, std::string(), 0);
}

std::optional<std::string> mappers::TitleTagMapper::getTagForTitle(const std::string& ck2Title, int eu4Capital)
{
	return getTagForTitle(ck2Title, std::string(), eu4Capital);
}

std::optional<std::string> mappers::TitleTagMapper::getTagForTitle(const std::string& ck2Title, const std::string& ck2BaseTitle, int eu4Capital)
{
	// the only case where we fail is on invalid invocation. Otherwise, failure is
	// not an option!
	if (ck2Title.empty())
		return std::nullopt;

	// The popes don't use proper titles and aren't registered.
	if (ck2Title == "The Pope")
	{
		if (!usedTags.count("PAP"))
		{
			usedTags.insert("PAP");
			return "PAP";
		}
		else
		{
			auto generatedTag = generateNewTag();
			usedTags.insert(generatedTag);
			return generatedTag;
		}
	}
	else if (ck2Title == "The Fraticelli Pope")
	{
		if (!usedTags.count("FAP"))
		{
			usedTags.insert("FAP");
			return "FAP";
		}
		else
		{
			auto generatedTag = generateNewTag();
			usedTags.insert(generatedTag);
			return generatedTag;
		}
	}

	// look up register
	const auto& registerItr = registeredTitleTags.find(ck2Title);
	if (registerItr != registeredTitleTags.end())
		return registerItr->second;

	// Attempt a capital match.
	if (eu4Capital)
	{
		for (const auto& mapping: theMappings)
		{
			const auto& match = mapping.capitalMatch(eu4Capital);
			if (match)
			{
				if (usedTags.count(*match))
					continue;
				registerTitle(ck2Title, *match);
				return *match;
			}
		}
	}

	// Attempt a title match
	for (const auto& mapping: theMappings)
	{
		const auto& match = mapping.titleMatch(ck2Title);
		if (match)
		{
			if (usedTags.count(*match))
				continue;
			registerTitle(ck2Title, *match);
			return *match;
		}
	}

	// Attempt a base title match (useful for custom empires)
	if (!ck2BaseTitle.empty())
	{
		for (const auto& mapping: theMappings)
		{
			const auto& match = mapping.titleMatch(ck2BaseTitle);
			if (match)
			{
				if (usedTags.count(*match))
					continue;
				registerTitle(ck2Title, *match);
				return *match;
			}
		}
	}

	// Generate a new tag
	auto generatedTag = generateNewTag();
	registerTitle(ck2Title, generatedTag);
	return generatedTag;
}

std::string mappers::TitleTagMapper::generateNewTag()
{
	std::ostringstream generatedEU4TagStream;
	generatedEU4TagStream << generatedEU4TagPrefix << std::setfill('0') << std::setw(2) << generatedEU4TagSuffix;
	const auto& eu4Tag = generatedEU4TagStream.str();
	const std::set<char> reservedPrefixes = {'D', 'C', 'K', 'E', 'T', 'O'};

	++generatedEU4TagSuffix;
	if (generatedEU4TagSuffix > 99)
	{
		generatedEU4TagSuffix = 0;
		--generatedEU4TagPrefix;
		while (reservedPrefixes.count(generatedEU4TagPrefix))
			--generatedEU4TagPrefix;
	}

	return eu4Tag;
}

std::optional<std::string> mappers::TitleTagMapper::getChinaForTitle(const std::string& ck2Title)
{
	// Try regular maps.
	for (const auto& mapping: chineseMappings)
	{
		const auto& match = mapping.titleMatch(ck2Title);
		if (match)
		{
			return *match;
		}
	}

	// Try for fallback
	for (const auto& mapping: chineseMappings)
	{
		const auto match = mapping.fallbackMatch();
		if (match)
		{
			return mapping.getEU4Tag();
		}
	}

	// No china?
	return std::nullopt;
}

std::set<std::string> mappers::TitleTagMapper::getAllChinas() const
{
	std::set<std::string> toReturn;
	for (const auto& mapping: chineseMappings)
		toReturn.insert(mapping.getEU4Tag());
	return toReturn;
}
