#include "ProvinceMapper.h"
#include "../../Common/Version.h"
#include "../../Configuration/Configuration.h"
#include "Log.h"
#include "ParserHelpers.h"
#include "ProvinceMapping.h"
#include <filesystem>
#include <fstream>
#include <stdexcept>
namespace fs = std::filesystem;

mappers::ProvinceMapper::ProvinceMapper()
{
	LOG(LogLevel::Info) << "-> Parsing province mappings";
	registerKeys();
	parseFile("configurables/province_mappings.txt");
	clearRegisteredKeywords();
	createMappings();
	LOG(LogLevel::Info) << "<> " << theMappings.getMappings().size() << " mappings loaded.";
}

mappers::ProvinceMapper::ProvinceMapper(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
	createMappings();
}

void mappers::ProvinceMapper::registerKeys()
{
	registerRegex("[0-9\\.]+", [this](const std::string& unused, std::istream& theStream) {
		// We support only a single, current version, so eu4-vic2 style multiple versions
		// have been cut. There should only be a single, 0.0.0.0={} block inside province_mappings.txt
		theMappings = ProvinceMappingsVersion(theStream);
	});
	registerRegex("[a-zA-Z0-9\\_.:]+", commonItems::ignoreItem);
}

void mappers::ProvinceMapper::createMappings()
{
	for (const auto& mapping: theMappings.getMappings())
	{
		// fix deliberate errors where we leave mappings without keys (asian wasteland comes to mind):
		if (mapping.getCK2Provinces().empty())
			continue;
		if (mapping.getEU4Provinces().empty())
			continue;

		for (const auto& ck2Number: mapping.getCK2Provinces())
		{
			if (ck2Number)
				CK2ToEU4ProvinceMap.insert(std::make_pair(ck2Number, mapping.getEU4Provinces()));
		}
		for (const auto& eu4Number: mapping.getEU4Provinces())
		{
			if (eu4Number)
				EU4ToCK2ProvinceMap.insert(std::make_pair(eu4Number, mapping.getCK2Provinces()));
		}
	}
}

std::vector<int> mappers::ProvinceMapper::getCK2ProvinceNumbers(const int eu4ProvinceNumber) const
{
	const auto& mapping = EU4ToCK2ProvinceMap.find(eu4ProvinceNumber);
	if (mapping != EU4ToCK2ProvinceMap.end())
		return mapping->second;
	return std::vector<int>();
}

std::vector<int> mappers::ProvinceMapper::getEU4ProvinceNumbers(int ck2ProvinceNumber) const
{
	const auto& mapping = CK2ToEU4ProvinceMap.find(ck2ProvinceNumber);
	if (mapping != CK2ToEU4ProvinceMap.end())
		return mapping->second;
	return std::vector<int>();
}

void mappers::ProvinceMapper::determineValidProvinces(const Configuration& theConfiguration)
{
	LOG(LogLevel::Info) << "-> Loading Valid Provinces";
	std::ifstream definitionFile(fs::u8path(theConfiguration.getEU4Path() + "/map/definition.csv"));
	if (!definitionFile.is_open())
		throw std::runtime_error("Could not open <eu4>/map/definition.csv");

	char input[256];
	while (!definitionFile.eof())
	{
		definitionFile.getline(input, 255);
		std::string inputStr(input);
		if (inputStr.substr(0, 8) == "province" || inputStr.substr(inputStr.find_last_of(';') + 1, 6) == "Unused" ||
			 inputStr.substr(inputStr.find_last_of(';') + 1, 3) == "RNW" || inputStr.size() < 2)
		{
			continue;
		}
		auto provNum = std::stoi(inputStr.substr(0, inputStr.find_first_of(';')));
		validEU4Provinces.insert(provNum);
	}
	LOG(LogLevel::Info) << "<> " << validEU4Provinces.size() << " valid provinces located.";
}