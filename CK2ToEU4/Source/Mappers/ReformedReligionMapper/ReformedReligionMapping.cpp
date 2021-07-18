#include "ReformedReligionMapping.h"
#include "CommonRegexes.h"
#include "ParserHelpers.h"
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
	registerKeyword("color", [this](const std::string& unused, std::istream& theStream) {
		color = commonItems::Color::Factory().getColor(theStream);
	});
	registerKeyword("country", [this](const std::string& mods, std::istream& theStream) {
		countryModifiers = commonItems::stringOfItem(theStream).getString();
		countryModifiers = countryModifiers.substr(3, countryModifiers.length() - 4);
	});
	registerKeyword("province", [this](const std::string& mods, std::istream& theStream) {
		provinceModifiers = commonItems::stringOfItem(theStream).getString();
		provinceModifiers = provinceModifiers.substr(3, provinceModifiers.length() - 4);
	});
	registerKeyword("country_as_secondary", [this](const std::string& mods, std::istream& theStream) {
		secondary = commonItems::stringOfItem(theStream).getString();
		secondary = secondary.substr(3, secondary.length() - 4);
	});
	registerKeyword("unique_modifiers", [this](const std::string& mods, std::istream& theStream) {
		uniqueMechanics = commonItems::stringOfItem(theStream).getString();
		uniqueMechanics = uniqueMechanics.substr(3, uniqueMechanics.length() - 4);
	});
	registerKeyword("not_unique_modifiers", [this](const std::string& mods, std::istream& theStream) {
		nonUniqueMechanics = commonItems::stringOfItem(theStream).getString();
		nonUniqueMechanics = nonUniqueMechanics.substr(3, nonUniqueMechanics.length() - 4);
	});
	registerKeyword("heretic", [this](const std::string& mods, std::istream& theStream) {
		hereticStrings = commonItems::stringOfItem(theStream).getString();
		hereticStrings = hereticStrings.substr(3, hereticStrings.length() - 4);
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