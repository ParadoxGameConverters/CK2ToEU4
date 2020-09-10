#include "Log.h"
#include "ParserHelpers.h"
#include "ReformedReligionMapping.h"
#include <iomanip>

mappers::ReformedReligionMapping::ReformedReligionMapping(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);	
	clearRegisteredKeywords();
}

void mappers::ReformedReligionMapping::registerKeys()
{
	registerKeyword("icon", [this](const std::string& unused, std::istream& theStream) {
		commonItems::singleInt icon(theStream);
		iconNumber = icon.getInt();
	});
	registerKeyword("color", [this](const std::string& mods, std::istream& theStream) {
		color = commonItems::singleItem(mods, theStream);
		color = color.substr(1, color.length()-2);
	});
	registerKeyword("country", [this](const std::string& mods, std::istream& theStream) {
		countryModifiers = commonItems::singleItem(mods, theStream);
		countryModifiers = countryModifiers.substr(1, countryModifiers.length()-2);
	});
	registerKeyword("province", [this](const std::string& mods, std::istream& theStream) {
		provinceModifiers = commonItems::singleItem(mods, theStream);
		provinceModifiers = provinceModifiers.substr(1, provinceModifiers.length()-2);
	});
	registerKeyword("country_as_secondary", [this](const std::string& mods, std::istream& theStream) {
		secondary = commonItems::singleItem(mods, theStream);
		secondary = secondary.substr(1, secondary.length()-2);
	});
	registerKeyword("unique_modifiers", [this](const std::string& mods, std::istream& theStream) {
		uniqueMechanics = commonItems::singleItem(mods, theStream);
		uniqueMechanics = uniqueMechanics.substr(1, uniqueMechanics.length()-2);
	});
	registerKeyword("not_unique_modifiers", [this](const std::string& mods, std::istream& theStream) {
		nonUniqueMechanics = commonItems::singleItem(mods, theStream);
		nonUniqueMechanics = nonUniqueMechanics.substr(1, nonUniqueMechanics.length()-2);
	});
	registerKeyword("heretic", [this](const std::string& mods, std::istream& theStream) {
		hereticStrings = commonItems::singleItem(mods, theStream);
		hereticStrings = hereticStrings.substr(1, hereticStrings.length()-2);
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}

void mappers::ReformedReligionMapping::addCountryModifiers(std::string mod)
{
	if (countryModifiers.find(mod) == std::string::npos && mod.length()) // Prevents duplicates
		countryModifiers = countryModifiers + "\n" + mod;
}
void mappers::ReformedReligionMapping::addProvinceModifiers(std::string mod)
{
	if (provinceModifiers.find(mod) == std::string::npos && mod.length()) // Prevents duplicates
		provinceModifiers = provinceModifiers + "\n" + mod;
}
void mappers::ReformedReligionMapping::addSecondary(std::string mod)
{
	if (secondary.find(mod) == std::string::npos && mod.length()) // Prevents duplicates
		secondary = secondary + "\n" + mod;
}
void mappers::ReformedReligionMapping::addNonUniqueMechanics(std::string mod)
{
	if (nonUniqueMechanics.find(mod) == std::string::npos && mod.length()) // Prevents duplicates
		nonUniqueMechanics = nonUniqueMechanics + "\n" + mod;
}