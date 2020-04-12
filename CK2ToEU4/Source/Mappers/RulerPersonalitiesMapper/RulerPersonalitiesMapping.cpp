#include "RulerPersonalitiesMapping.h"
#include "ParserHelpers.h"

mappers::RulerPersonalitiesMapping::RulerPersonalitiesMapping(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::RulerPersonalitiesMapping::registerKeys()
{
	registerKeyword("[a-z_]+", [this](const std::string& personality, std::istream& theStream) {
		const commonItems::singleInt valueInt(theStream);
		personalities.insert(std::pair(personality, valueInt.getInt()));
	});
	registerRegex("[a-zA-Z0-9\\_.:-]+", commonItems::ignoreItem);
}

int mappers::RulerPersonalitiesMapping::evaluatePersonality(const std::pair<int, std::shared_ptr<CK2::Character>>& theCharacter) const
{
}
