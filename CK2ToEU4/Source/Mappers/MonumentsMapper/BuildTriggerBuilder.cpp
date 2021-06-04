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