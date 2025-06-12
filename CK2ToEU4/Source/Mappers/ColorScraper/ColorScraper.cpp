#include "ColorScraper.h"
#include "CommonRegexes.h"
#include "ParserHelpers.h"

void mappers::ColorScraper::scrapeColors(const std::filesystem::path& filePath)
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
	registerRegex(R"((e|k|d|c)_[A-Za-z0-9_\-]+)", [this](const std::string& titleName, std::istream& theStream) {
		ColorScraper newScraper;
		newScraper.scrapeColors(theStream, titleName);
		auto foundColors = newScraper.getColors();
		for (const auto& foundColor: foundColors)
		{
			if (!foundColor.second)
				continue;
			titleColors[foundColor.first] = foundColor.second; // Overwriting for mod sources
		}
	});

	registerKeyword("color", [this](const std::string& unused, std::istream& theStream) {
		color = commonItems::Color::Factory{}.getColor(theStream);
	});

	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}

std::optional<commonItems::Color> mappers::ColorScraper::getColorForTitle(const std::string& titleName) const
{
	const auto& titleItr = titleColors.find(titleName);
	if (titleItr != titleColors.end())
		return titleItr->second;
	return std::nullopt;
}