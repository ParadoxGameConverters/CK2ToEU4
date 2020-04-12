#include "RulerPersonalitiesMapper.h"
#include "Log.h"
#include "ParserHelpers.h"

mappers::RulerPersonalitiesMapper::RulerPersonalitiesMapper()
{
	LOG(LogLevel::Info) << "-> Parsing Ruler Personalities";
	registerKeys();
	parseFile("configurables/ruler_personalities.txt");
	clearRegisteredKeywords();
	LOG(LogLevel::Info) << "<> " << theMappings.size() << " personalities loaded.";
}

mappers::RulerPersonalitiesMapper::RulerPersonalitiesMapper(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::RulerPersonalitiesMapper::registerKeys()
{
	registerKeyword("[a-z_]+", [this](const std::string& personality, std::istream& theStream)
	{
		const auto newMapping = RulerPersonalitiesMapping(theStream);
		theMappings.insert(std::pair(personality, newMapping));
	});
	registerRegex("[a-zA-Z0-9\\_.:-]+", commonItems::ignoreItem);
}

std::set<std::string> mappers::RulerPersonalitiesMapper::getPersonalities(const std::pair<int, std::shared_ptr<CK2::Character>>& theCharacter) const
{
}
