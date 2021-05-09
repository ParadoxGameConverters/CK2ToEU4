#include "Log.h"
#include "ParserHelpers.h"
#include "MonumentsMapper.h"
#include <iomanip>
#include <set>

mappers::MonumentsMapper::MonumentsMapper()
{
	LOG(LogLevel::Info) << "-> Parsing Monuments mappings";
	registerKeys();
	parseFile("configurables/monuments_mappings.txt");
	clearRegisteredKeywords();
	LOG(LogLevel::Info) << "<> " << " Monuments mapped.";
}

mappers::MonumentsMapper::MonumentsMapper(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::MonumentsMapper::registerKeys()
{
	registerRegex(commonItems::catchallRegex, [this](const std::string& type, std::istream& theStream) {
		MonumentsMapping newMapping(theStream);
		wonders.emplace(std::pair(type, newMapping));
	});
}
