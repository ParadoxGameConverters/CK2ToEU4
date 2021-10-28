#include "MonumentsMapping.h"
#include "BuildTriggerBuilder.h"
#include "CommonRegexes.h"
#include "InternalModifiers.h"
#include "ParserHelpers.h"

mappers::MonumentsMapping::MonumentsMapping(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::MonumentsMapping::registerKeys()
{
	registerKeyword("properties", [this](std::istream& theStream) {
		isBase = true; // Only the wonder bases have properties
		auto movedStr = commonItems::stringOfItem(theStream).getString();
		canBeMoved = movedStr.find("can_be_moved = yes") != std::string::npos;
	});
	registerKeyword("build_trigger", [this](std::istream& theStream) {
		const BuildTriggerBuilder builder(theStream);
		buildTrigger = "OR = {\n\t\t\t";
		buildTrigger += builder.getBuildTrigger();
		cultural = builder.getCultural();
		religious = builder.getReligious();
	});
	registerKeyword("province_modifiers", [this](std::istream& theStream) {
		AddProvinceSet(theStream);
	});
	registerKeyword("area_modifiers", [this](std::istream& theStream) {
		AddAreaSet(theStream);
	});
	registerKeyword("country_modifiers", [this](std::istream& theStream) {
		AddCountrySet(theStream);
	});
	registerKeyword("on_upgraded", [this](std::istream& theStream) {
		auto tempInput = commonItems::stringOfItem(theStream).getString();
		tempInput = tempInput.substr(tempInput.find('{') + 1, tempInput.length());
		tempInput = tempInput.substr(0, tempInput.find('}'));
		onUpgraded += tempInput;
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}

void mappers::MonumentsMapping::AddProvinceSet(std::istream& theStream)
{
	const InternalModifiers mods(theStream);
	for (const auto& tempMod: mods.getModifierMap())
		if (!provinceModifiers.contains(tempMod.first))
			provinceModifiers.emplace(tempMod.first, tempMod.second);
}
void mappers::MonumentsMapping::AddAreaSet(std::istream& theStream)
{
	const InternalModifiers mods(theStream);
	for (const auto& tempMod: mods.getModifierMap())
		if (!areaModifiers.contains(tempMod.first))
			areaModifiers.emplace(tempMod.first, tempMod.second);
}
void mappers::MonumentsMapping::AddCountrySet(std::istream& theStream)
{
	const InternalModifiers mods(theStream);
	for (const auto& tempMod: mods.getModifierMap())
		if (!countryModifiers.contains(tempMod.first))
			countryModifiers.emplace(tempMod.first, tempMod.second);
}