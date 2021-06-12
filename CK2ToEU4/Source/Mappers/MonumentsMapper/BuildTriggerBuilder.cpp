#include "Log.h"
#include "ParserHelpers.h"
#include "BuildTriggerBuilder.h"
#include <iomanip>
#include <set>
#include "CommonRegexes.h"

mappers::BuildTriggerBuilder::BuildTriggerBuilder()
{
	registerKeys();
	clearRegisteredKeywords();
}

mappers::BuildTriggerBuilder::BuildTriggerBuilder(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
	buildTrigger += "\n\t}";
}

void mappers::BuildTriggerBuilder::registerKeys()
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
			buildTrigger += ("AND = {\n\t\t\t\tculture_group = " + group + "\n\t\t\t\thas_owner_culture = yes\n\t\t\t}\n\t\t");
		}
	});
	registerKeyword("cultural", [this](const std::string& mods, std::istream& theStream) {
		cultural = true;
	});
	registerKeyword("religious", [this](const std::string& mods, std::istream& theStream) {
		religious = true;
	});
	registerKeyword("other", [this](const std::string& mods, std::istream& theStream) {
		auto tempInput = commonItems::stringOfItem(theStream).getString();
		tempInput = tempInput.substr(tempInput.find('{') + 1, tempInput.length());
		tempInput = tempInput.substr(0, tempInput.find_last_of('}'));
		buildTrigger += tempInput;
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}