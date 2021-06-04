#include "ParserHelpers.h"
#include "MonumentsMapping.h"
#include "InternalModifiers.h"
#include <iomanip>
#include "CommonRegexes.h"

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
		buildTrigger = "OR = {\n\t\t";
		CreateBuildTrigger(theStream);
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
		onUpgraded += commonItems::stringOfItem(theStream).getString();

		onUpgraded = onUpgraded.substr(onUpgraded.find('{')+1, onUpgraded.length());
		onUpgraded = onUpgraded.substr(0, onUpgraded.find('}'));
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}
void mappers::MonumentsMapping::CreateBuildTrigger(std::istream& theStream)
{	
	registerKeyword("religious_groups", [this](const std::string& mods, std::istream& theStream) {
		const auto& groups = commonItems::stringList(theStream).getStrings();
		for (auto& group: groups)
		{
			buildTrigger += ("AND = {\n\t\t\t\treligion_group = " + group + "\n\t\t\t\thas_owner_religion = yes\n\t\t\t}\n\t\t");
		}
	});
	registerKeyword("cultural_groups", [this](const std::string& mods, std::istream& theStream) {
		const auto& groups = commonItems::stringList(theStream).getStrings();
		for (auto& group: groups)
		{
			buildTrigger += ("AND = {\n\t\t\t\tcultural_groups = " + group + "\n\t\t\t\thas_owner_culture = yes\n\t\t\t}\n\t\t");
		}
	});
	registerKeyword("cultural", [this](const std::string& mods, std::istream& theStream) {
		cultural = true;
	});
	registerKeyword("religious", [this](const std::string& mods, std::istream& theStream) {
		religious = true;
	});
	registerKeyword("other", [this](const std::string& mods, std::istream& theStream) {
		buildTrigger += commonItems::stringOfItem(theStream).getString();

		buildTrigger = buildTrigger.substr(buildTrigger.find('{') + 1, buildTrigger.length());
		buildTrigger = buildTrigger.substr(0, buildTrigger.find('}'));
	});
	buildTrigger += "\n\t}";
}
void mappers::MonumentsMapping::AddProvinceSet(std::istream& theStream)
{
	mappers::InternalModifiers mods(theStream);
	const auto& tempMod = mods.getModifierType();
	const auto& tempValues = mods.getModifierValues();

	if (!provinceModifiers.contains(tempMod))
		provinceModifiers.emplace(tempMod, tempValues);
}
void mappers::MonumentsMapping::AddAreaSet(std::istream& theStream)
{
	mappers::InternalModifiers mods(theStream);
	const auto& tempMod = mods.getModifierType();
	const auto& tempValues = mods.getModifierValues();
	
	if (!areaModifiers.contains(tempMod))
		areaModifiers.emplace(tempMod, tempValues);
}
void mappers::MonumentsMapping::AddCountrySet(std::istream& theStream)
{
	mappers::InternalModifiers mods(theStream);
	const auto& tempMod = mods.getModifierType();
	const auto& tempValues = mods.getModifierValues();

	if (!countryModifiers.contains(tempMod))
		countryModifiers.emplace(tempMod, tempValues);
}