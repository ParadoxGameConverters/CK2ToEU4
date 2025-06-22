#include "MonumentsMapper.h"
#include "CommonRegexes.h"
#include "Log.h"
#include <iomanip>
#include <set>

mappers::MonumentsMapper::MonumentsMapper()
{
	Log(LogLevel::Info) << "-> Parsing Monuments mappings";
	registerKeys();
	parseFile("configurables/monuments_mappings.txt");
	clearRegisteredKeywords();
	Log(LogLevel::Info) << "<> " << wonders.size() << " Monuments mapped.";
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
