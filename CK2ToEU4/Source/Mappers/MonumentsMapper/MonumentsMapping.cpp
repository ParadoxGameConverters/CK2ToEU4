#include "ParserHelpers.h"
#include "MonumentsMapping.h"
#include "InternalModifiers.h"
#include "BuildTriggerBuilder.h"
#include <iomanip>
#include "CommonRegexes.h"
#include "Log.h"

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
		/*auto movedStr = commonItems::stringOfItem(theStream).getString();
		canBeMoved = (movedStr.find("can_be_moved = yes") != std::string::npos);*/
	});
	registerKeyword("build_trigger", [this](const std::string& mods, std::istream& theStream) {
		buildTrigger = "OR = {\n\t\t";
		BuildTriggerBuilder builder(theStream);
		buildTrigger += builder.getBuildTrigger();
		cultural = builder.getCultural();
		religious = builder.getReligious();
	});
	registerKeyword("province_modifiers", [this](const std::string& mods, std::istream& theStream) {
		AddProvinceSet(theStream);
	});
	registerKeyword("area_modifiers", [this](const std::string& mods, std::istream& theStream) {
		AddAreaSet(theStream);
	});
	registerKeyword("country_modifiers", [this](const std::string& mods, std::istream& theStream) {
		AddCountrySet(theStream);
	});
	registerKeyword("on_upgraded", [this](const std::string& mods, std::istream& theStream) {
		auto tempInput = commonItems::stringOfItem(theStream).getString();
		tempInput = tempInput.substr(tempInput.find('{') + 1, tempInput.length());
		tempInput = tempInput.substr(0, tempInput.find('}'));		
		onUpgraded += tempInput;
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}
void mappers::MonumentsMapping::AddProvinceSet(std::istream& theStream)
{
	InternalModifiers mods(theStream);
	const auto& tempMod = mods.getModifierType();
	const auto& tempValues = mods.getModifierValues();

	if (!provinceModifiers.contains(tempMod))
		provinceModifiers.emplace(tempMod, tempValues);
}
void mappers::MonumentsMapping::AddAreaSet(std::istream& theStream)
{
	InternalModifiers mods(theStream);
	const auto& tempMod = mods.getModifierType();
	const auto& tempValues = mods.getModifierValues();
	
	if (!areaModifiers.contains(tempMod))
		areaModifiers.emplace(tempMod, tempValues);
}
void mappers::MonumentsMapping::AddCountrySet(std::istream& theStream)
{
	InternalModifiers mods(theStream);
	const auto& tempMod = mods.getModifierType();
	const auto& tempValues = mods.getModifierValues();

	if (!countryModifiers.contains(tempMod))
		countryModifiers.emplace(tempMod, tempValues);
}