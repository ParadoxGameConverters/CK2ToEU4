#include "BuildTriggerBuilder.h"
#include "CommonRegexes.h"
#include "ParserHelpers.h"

mappers::BuildTriggerBuilder::BuildTriggerBuilder(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
	buildTrigger += "\n\t}";
}

void mappers::BuildTriggerBuilder::registerKeys()
{
	registerKeyword("religious_groups", [this](std::istream& theStream) {
		const auto& groups = commonItems::getStrings(theStream);
		for (auto& group: groups)
			buildTrigger += "AND = {\n\t\t\t\treligion_group = " + group + "\n\t\t\t\thas_owner_religion = yes\n\t\t\t}\n\t\t";
	});
	registerKeyword("cultural_groups", [this](std::istream& theStream) {
		const auto& groups = commonItems::getStrings(theStream);
		for (auto& group: groups)
			buildTrigger += "AND = {\n\t\t\t\tculture_group = " + group + "\n\t\t\t\thas_owner_culture = yes\n\t\t\t}\n\t\t";
	});
	registerKeyword("cultural", [this](std::istream& theStream) {
		cultural = commonItems::getString(theStream) == "yes";
	});
	registerKeyword("religious", [this](std::istream& theStream) {
		religious = commonItems::getString(theStream) == "yes";
	});
	registerKeyword("other", [this](std::istream& theStream) {
		auto tempInput = commonItems::stringOfItem(theStream).getString();
		tempInput = tempInput.substr(tempInput.find('{') + 1, tempInput.length());
		tempInput = tempInput.substr(0, tempInput.find_last_of('}'));
		buildTrigger += tempInput;
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}