#include "Log.h"
#include "ParserHelpers.h"
#include "ReformedReligionMapper.h"
#include <iomanip>
#include <set>

mappers::ReformedReligionMapper::ReformedReligionMapper()
{
	LOG(LogLevel::Info) << "-> Parsing Reformed Religion mappings";
	registerKeys();
	parseFile("configurables/reformed_religions_mappings.txt");
	parseFile("configurables/pagan_religions.txt");
	//parseFile("configurables/reformation/oldPagans/03_pagan_religions.txt");
	//parseFile("configurables/reformation/oldPagans/04_replaced_religions.txt");
	clearRegisteredKeywords();
	LOG(LogLevel::Info) << "<> " << " Religions mapped.";
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
