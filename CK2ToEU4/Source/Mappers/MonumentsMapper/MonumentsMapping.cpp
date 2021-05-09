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
		onUpgraded = commonItems::singleString(theStream).getString();
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}
void mappers::MonumentsMapping::CreateBuildTrigger(std::istream& theStream)
{	
	registerKeyword("religious_groups", [this](const std::string& mods, std::istream& theStream) {
		const auto groups = commonItems::stringList(theStream).getStrings();
		for (auto& group: groups)
		{
			buildTrigger += ("AND = {\n\t\t\t\treligion_group = " + group + "\n\t\t\t\thas_owner_religion = yes\n\t\t\t}\n\t\t");
		}
	});
	registerKeyword("cultural_groups", [this](const std::string& mods, std::istream& theStream) {
		const auto groups = commonItems::stringList(theStream).getStrings();
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
		buildTrigger += commonItems::singleString(theStream).getString();
	});
	buildTrigger += "\n\t}";
}
void mappers::MonumentsMapping::AddProvinceSet(std::istream& theStream)
{
	registerRegex(commonItems::catchallRegex, [this](const std::string& mods, std::istream& theStream) {
		if (numOfModifiers < 5 && !provinceModifiers.count(mods))
		{
			provinceModifiers.emplace(mods, commonItems::doubleList(theStream).getDoubles());
			numOfModifiers++;
		}
	});
}
void mappers::MonumentsMapping::AddAreaSet(std::istream& theStream)
{
	registerRegex(commonItems::catchallRegex, [this](const std::string& mods, std::istream& theStream) {
		if (numOfModifiers < 5 && !provinceModifiers.count(mods))
		{
			areaModifiers.emplace(mods, commonItems::doubleList(theStream).getDoubles());
			numOfModifiers++;
		}
	});
}
void mappers::MonumentsMapping::AddCountrySet(std::istream& theStream)
{
	registerRegex(commonItems::catchallRegex, [this](const std::string& mods, std::istream& theStream) {
		if (numOfModifiers < 5 && !provinceModifiers.count(mods))
		{
			countryModifiers.emplace(mods, commonItems::doubleList(theStream).getDoubles());
			numOfModifiers++;
		}
	});
}