#include "outCountry.h"

std::ostream& EU4::operator<<(std::ostream& output, const Country& country)
{
	output << "government = " << country.details.government << "\n";
	if (!country.details.reforms.empty()) {
		for (const auto& reform: country.details.reforms) { output << "add_government_reform = " << reform << "\n"; }
	}
	output << "government_rank = " << country.details.governmentRank << "\n";
	output << "technology_group = " << country.details.technologyGroup << "\n";
	output << "religion = " << country.details.religion << "\n";
	output << "primary_culture = " << country.details.primaryCulture << "\n";
	if (country.details.capital) output << "capital = " << country.details.capital << "\n";
	if (country.details.fixedCapital) output << "fixed_capital = yes\n";
	if (country.details.mercantilism) output << "mercantilism = " << country.details.mercantilism << "\n";
	if (!country.details.unitType.empty()) output << "unit_type = " << country.details.unitType << "\n";
	if (!country.details.religiousSchool.empty()) output << "religious_school = " << country.details.religiousSchool << "\n";
	if (!country.details.cults.empty()) {
		for (const auto& cult: country.details.cults) { output << "unlock_cult = " << cult << "\n"; }
	}
	if (!country.details.acceptedCultures.empty()) {
		for (const auto& culture: country.details.acceptedCultures) { output << "add_accepted_culture = " << culture << "\n"; }
	}
	if (country.details.armyProfessionalism != 0) output << "add_army_professionalism = " << country.details.armyProfessionalism << "\n";
	if (!country.details.historicalRivals.empty()) {
		for (const auto& rival: country.details.historicalRivals) { output << "historical_rival = " << rival << "\n"; }
	}
	if (!country.details.historicalFriends.empty()) {
		for (const auto& friendd: country.details.historicalFriends) { output << "historical_friend = " << friendd << "\n"; }
	}
	if (!country.details.nationalFocus.empty()) output << "national_focus = " << country.details.nationalFocus << "\n";

	if (country.details.piety != 0) output << "add_piety = " << country.details.piety << "\n";
	if (country.details.elector) output << "elector = yes\n";
	if (!country.details.secondaryReligion.empty()) output << "secondary_religion = " << country.details.secondaryReligion << "\n";
	if (!country.details.harmonizedReligions.empty()) {
		for (const auto& religion: country.details.harmonizedReligions) { output << "add_harmonized_religion = " << religion << "\n"; }
	}

	if (country.details.monarch.isSet) {
		output << country.conversionDate << "= {\n";
		output << "\tmonarch = {\n";
		output << country.details.monarch;
		output << "\t}\n";
		for (const auto& personality: country.details.monarch.personalities)
			output << "\tadd_ruler_personality = " << personality << "\n";
		if (country.details.queen.isSet) {
			output << "\tqueen = {\n";
			output << country.details.queen;
			output << "\t}\n";
			for (const auto& personality: country.details.queen.personalities)
				output << "\tadd_queen_personality = " << personality << "\n";
		}
		if (country.details.heir.isSet) {
			output << "\their = {\n";
			output << country.details.heir;
			output << "\t}\n";
			for (const auto& personality: country.details.heir.personalities)
				output << "\tadd_heir_personality = " << personality << "\n";
		}
		output << "}\n";
	}

	// this is done only for countries without a title - vanilla tags where we're regurgitating history ad verbatim.
	if (country.getTitle().first.empty() && !country.details.historyLessons.empty()) {
		for (const auto& historyLesson: country.details.historyLessons) {
			output << historyLesson.first << " = {\n";
			output << historyLesson.second;
			output << "}\n\n";
		}
	}

	return output;
}

void EU4::Country::outputCommons(std::ostream& output)
{
	output << "graphical_culture = " << details.graphicalCulture << "\n";
	output << "color = { " << details.color << " }\n";
	if (details.revolutionaryColor) output << "revolutionary_colors = { " << details.revolutionaryColor << " }\n";
	if (!details.historicalIdeaGroups.empty()) {
		output << "historical_idea_groups = {\n";
		for (const auto& idea: details.historicalIdeaGroups) { output << "\t" << idea << "\n"; }
		output << "}\n";
	}
	if (details.historicalScore) output << "historical_score = " << details.historicalScore << "\n";
	if (details.randomChance) output << "random_nation_chance = 0\n";
	if (!details.historicalUnits.empty()) {
		output << "historical_units = {\n";
		for (const auto& unit: details.historicalUnits) { output << "\t" << unit << "\n"; }
		output << "}\n";
	}
	if (!details.monarchNames.empty()) {
		output << "monarch_names = {\n";
		for (const auto& name: details.monarchNames) { output << "\t\"" << name.first << " #" << name.second.first << "\" = " << name.second.second << "\n"; }
		output << "}\n";
	}
	if (!details.leaderNames.empty()) {
		output << "leader_names = {\n";
		for (const auto& name: details.leaderNames) { output << "\t\"" << name << "\"\n"; }
		output << "}\n";
	}
	if (!details.shipNames.empty()) {
		output << "ship_names = {\n";
		for (const auto& name: details.shipNames) { output << "\t\"" << name << "\"\n"; }
		output << "}\n";
	}
	if (!details.armyNames.empty()) {
		output << "army_names = {\n";
		for (const auto& name: details.armyNames) { output << "\t\"" << name << "\"\n"; }
		output << "}\n";
	}
	if (!details.fleetNames.empty()) {
		output << "fleet_names = {\n";
		for (const auto& name: details.fleetNames) { output << "\t\"" << name << "\"\n"; }
		output << "}\n";
	}
	if (!details.preferredReligion.empty()) output << "preferred_religion = " << details.preferredReligion << "\n";
	if (!details.colonialParent.empty()) output << "colonial_parent = " << details.colonialParent << "\n";
	if (!details.specialUnitCulture.empty()) output << "special_unit_culture = " << details.specialUnitCulture << "\n";
	if (details.all_your_core_are_belong_to_us) output << "all_your_core_are_belong_to_us = yes\n";
	if (details.rightToBEARArms) output << "right_to_bear_arms = yes\n";
}

std::ostream& EU4::operator<<(std::ostream& output, const Monarch& monarch)
{
	output << "\t\tname = \"" << monarch.name << "\"\n";
	if (!monarch.dynasty.empty()) { output << "\t\tdynasty = \"" << monarch.dynasty << "\"\n"; }
	output << "\t\tadm = " << monarch.adm << "\n";
	output << "\t\tdip = " << monarch.dip << "\n";
	output << "\t\tmil = " << monarch.mil << "\n";
	if (!monarch.originCountry.empty()) output << "\t\tcountry_of_origin = " << monarch.originCountry << "\n";
	output << "\t\tbirth_date = " << monarch.birthDate << "\n";
	if (monarch.regency) output << "\t\tregent = yes\n";
	if (monarch.deathDate != date("1.1.1")) output << "\t\tdeath_date = " << monarch.deathDate << "\n";
	if (monarch.female) output << "\t\tfemale = yes\n";
	if (monarch.claim) output << "\t\tclaim = " << monarch.claim << "\n";
	output << "\t\treligion = " << monarch.religion << "\n";
	output << "\t\tculture = " << monarch.culture << "\n";
	return output;
}
