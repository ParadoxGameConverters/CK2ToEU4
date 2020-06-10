#include "ColorScraper.h"
#include "Log.h"
#include "ParserHelpers.h"

void mappers::ColorScraper::scrapeColors(const std::string& filePath)
{
	registerKeys();
	parseFile(filePath);
	clearRegisteredKeywords();
}

void mappers::ColorScraper::scrapeColors(std::istream& theStream, std::string theName)
{
	name = std::move(theName);
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
	titleColors.insert(std::pair(name, color));
}

void mappers::ColorScraper::registerKeys()
{
	registerRegex("(e|k|d|c)_[A-Za-z0-9_-]+", [this](const std::string& titleName, std::istream& theStream) {
		ColorScraper newScraper;
		newScraper.scrapeColors(theStream, titleName);
		auto foundColors = newScraper.getColors();
		for (const auto& foundColor: foundColors)
		{
			if (!foundColor.second) continue;
			titleColors[foundColor.first] = foundColor.second; // Overwriting for mod sources
		}
	});

	registerRegex("color", [this](const std::string& unused, std::istream& theStream) {
		color = commonItems::Color(theStream);
	});

	registerRegex("[A-Za-z0-9\\_.:-]+", commonItems::ignoreItem);
}

std::optional<commonItems::Color> mappers::ColorScraper::getColorForTitle(const std::string& titleName) const
{
	const auto& titleItr = titleColors.find(titleName);
	if (titleItr != titleColors.end())
		return titleItr->second;		
	return std::nullopt;
}