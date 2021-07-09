#include "ReformedReligionMapper.h"
#include "CommonRegexes.h"
#include "Log.h"
#include "ParserHelpers.h"
#include <iomanip>

mappers::ReformedReligionMapper::ReformedReligionMapper()
{
	Log(LogLevel::Info) << "-> Parsing Reformed Religion mappings";
	registerKeys();
	parseFile("configurables/reformed_religions_mappings.txt");
	parseFile("configurables/pagan_religions.txt");
	clearRegisteredKeywords();
	Log(LogLevel::Info) << "<> "
							  << " Religions mapped.";
}

mappers::ReformedReligionMapper::ReformedReligionMapper(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::ReformedReligionMapper::registerKeys()
{
	registerRegex(commonItems::catchallRegex, [this](const std::string& doctrine, std::istream& theStream) {
		ReformedReligionMapping newMapping(theStream);
		newMapping.setName(doctrine);
		religionEntries.insert(std::pair(doctrine, newMapping));
	});
}
