#include "Log.h"
#include "ParserHelpers.h"
#include "ReformedReligionMapping.h"
#include <iomanip>

mappers::ReformedReligionMapping::ReformedReligionMapping(std::istream& theStream)
{
	LOG(LogLevel::Debug) << "This is theStream: " << theStream.rdbuf();
	//registerKeys();
	//parseStream(theStream);	
	clearRegisteredKeywords();
}

void mappers::ReformedReligionMapping::registerKeys()
{
	registerKeyword("icon", [this](const std::string& unused, std::istream& theStream) {
		commonItems::singleInt icon(theStream);
		iconNumber = icon.getInt();
	});
	registerKeyword("color", [this](const std::string& unused, std::istream& theStream) {
		commonItems::singleString colors(theStream);
		color = colors.getString();
	});
	registerKeyword("country", [this](const std::string& mods, std::istream& theStream) {
		countryModifiers = commonItems::singleItem(mods, theStream);
	});
	registerKeyword("province", [this](const std::string& mods, std::istream& theStream) {
		provinceModifiers = commonItems::singleItem(mods, theStream);
	});
	registerKeyword("unique_modifiers", [this](const std::string& mods, std::istream& theStream) {
		uniqueMechanics = commonItems::singleItem(mods, theStream);
	});
	registerKeyword("not_unique_modifiers", [this](const std::string& mods, std::istream& theStream) {
		nonUniqueMechanics = commonItems::singleItem(mods, theStream);
	});
	registerKeyword("heretic", [this](const std::string& mods, std::istream& theStream) {
		hereticStrings = commonItems::singleItem(mods, theStream);
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}