#include "ParserHelpers.h"
#include "MonumentsMapping.h"
#include <iomanip>

mappers::MonumentsMapping::MonumentsMapping(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::MonumentsMapping::registerKeys()
{
	registerKeyword("properties", [this](const std::string& mods, std::istream& theStream) {
		isBase = true; // Only the wonder bases have properties
		const commonItems::singleString movedStr(theStream);
		canBeMoved = movedStr.getString() == "can_be_moved = yes";
	});
	registerKeyword("build_trigger", [this](const std::string& mods, std::istream& theStream) {
		CreateBuildTrigger(theStream);
		numOfModifiers++;
	});
	registerKeyword("province_modifiers", [this](const std::string& mods, std::istream& theStream) {
		AddModifiers(theStream);
	});
	registerKeyword("area_modifiers", [this](const std::string& mods, std::istream& theStream) {
		desc = commonItems::singleString(theStream).getString();
	});
	registerKeyword("country_modifiers", [this](const std::string& mods, std::istream& theStream) {
		if (commonItems::singleString(theStream).getString() == "yes")
			active = true;
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}
void mappers::MonumentsMapping::CreateBuildTrigger(std::istream& theStream)
{	
	registerKeyword("religious_groups", [this](const std::string& mods, std::istream& theStream) {
		const auto groups = commonItems::stringList(theStream).getStrings();
		for (auto& group: groups)
		{
			buildTrigger += ("religion_group = " + group + "\n\t\t");
		}
	});
	registerKeyword("cultural_groups", [this](const std::string& mods, std::istream& theStream) {
		const auto groups = commonItems::stringList(theStream).getStrings();
		for (auto& group: groups)
		{
			buildTrigger += ("cultural_groups = " + group + "\n\t\t");
		}
	});
	registerKeyword("cultural", [this](const std::string& mods, std::istream& theStream) {
		cultural = true;
	});
	registerKeyword("religious", [this](const std::string& mods, std::istream& theStream) {
		religious = true;
	});
	registerKeyword("other", [this](const std::string& mods, std::istream& theStream) {
		buildTrigger += commonItems::singleString(theStream).getString();
	});
	buildTrigger += "\n\t}";
}
void mappers::MonumentsMapping::AddModifiers(std::istream& theStream)
{
	registerKeyword("province_modifiers", [this](const std::string& mods, std::istream& theStream) {
		AddProvinceSet(theStream);
	});
	registerKeyword("area_modifiers", [this](const std::string& mods, std::istream& theStream) {
		AddAreaSet(theStream);
	});
	registerKeyword("country_modifiers", [this](const std::string& mods, std::istream& theStream) {
		AddCountrySet(theStream);
	});
}
void mappers::MonumentsMapping::AddProvinceSet(std::istream& theStream)
{
	registerRegex(commonItems::catchallRegex, [this](const std::string& mods, std::istream& theStream) {
		provinceModifiers.emplace(mods, commonItems::doubleList(theStream).getDoubles());
	});
}
void mappers::MonumentsMapping::AddAreaSet(std::istream& theStream)
{
	registerRegex(commonItems::catchallRegex, [this](const std::string& mods, std::istream& theStream) {
		areaModifiers.emplace(mods, commonItems::doubleList(theStream).getDoubles());
	});
}
void mappers::MonumentsMapping::AddCountrySet(std::istream& theStream)
{
	registerRegex(commonItems::catchallRegex, [this](const std::string& mods, std::istream& theStream) {
		countryModifiers.emplace(mods, commonItems::doubleList(theStream).getDoubles());
	});
}