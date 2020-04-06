#include "TitleTagMapper.h"
#include "Log.h"
#include "ParserHelpers.h"
#include <iomanip>

mappers::TitleTagMapper::TitleTagMapper()
{
	LOG(LogLevel::Info) << "-> Parsing Tag mappings";
	registerKeys();
	parseFile("configurables/tag_mappings.txt");
	clearRegisteredKeywords();
	LOG(LogLevel::Info) << "<> " << theMappings.size() << " mappings loaded.";
}

mappers::TitleTagMapper::TitleTagMapper(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::TitleTagMapper::registerKeys()
{
	registerKeyword("link", [this](const std::string& unused, std::istream& theStream) {
		theMappings.emplace_back(TitleTagMapping(theStream));
	});
	registerRegex("[a-zA-Z0-9\\_.:-]+", commonItems::ignoreItem);
}

void mappers::TitleTagMapper::registerTitle(const std::string& ck2title, const std::string& eu4tag)
{
	registeredTitleTags.insert(std::pair(ck2title, eu4tag));
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
				Log(LogLevel::Debug) << " Mapping " << ck2Title << "->" << *match << "\t\t[capitals]";
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
			Log(LogLevel::Debug) << " Mapping " << ck2Title << "->" << *match << "\t\t[title]";
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
				Log(LogLevel::Debug) << " Mapping " << ck2Title << "->" << *match << "\t\t[base title]";
				registerTitle(ck2Title, *match);
				return *match;
			}
		}
	}
	
	// Generate a new tag
	auto generatedTag = generateNewTag();
	Log(LogLevel::Debug) << " Mapping " << ck2Title << "->" << generatedTag;
	registerTitle(ck2Title, generatedTag);
	return generatedTag;
}

std::string mappers::TitleTagMapper::generateNewTag()
{
	std::ostringstream generatedEU4TagStream;
	generatedEU4TagStream << generatedEU4TagPrefix << std::setfill('0') << std::setw(2) << generatedEU4TagSuffix;
	const auto& eu4Tag = generatedEU4TagStream.str();

	++generatedEU4TagSuffix;
	if (generatedEU4TagSuffix > 99)
	{
		generatedEU4TagSuffix = 0;
		--generatedEU4TagPrefix;
	}

	return eu4Tag;
}
