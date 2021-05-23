#include "outMonument.h"

EU4::outMonument::outMonument(const Configuration& theConfiguration, std::optional<std::pair<int, std::shared_ptr<CK2::Wonder>>> wonder)
{
	std::ofstream output("output/" + theConfiguration.getOutputName() + " /common/great_projects/!00_converted_monuments.txt");
	if (!output.is_open())
		throw std::runtime_error("Could not create monuments file: output/" + theConfiguration.getOutputName() + "/common/great_projects/!00_converted_monuments.txt");

	mappers::MonumentsMapper monumentsMapper;
	mappers::MonumentsMapping monumentsMapping;

	std::string canBeMoved = "no";
	std::string builderTrigger;
	std::vector<std::string> onUpgraded;
	wonder->second->setTrueDate(wonder->second->getBinaryDate());

	std::map<std::string, std::vector<double>> provinceModifiers;
	std::map<std::string, std::vector<double>> areaModifiers;
	std::map<std::string, std::vector<double>> countryModifiers;
	short numOfModifiers = 0;
	
	// Goes through each upgrade that a wonder has and creates vectors for the bonuses, only done for up to a max of 4 bonuses.
	for (const auto& upgrade: wonder->second->getUpgrades())
	{
		bool addedMod = false;

		monumentsMapping = monumentsMapper.getWonders().find(upgrade)->second;
		if (monumentsMapping.getCanBeMoved())
			canBeMoved = "yes";
		if (!monumentsMapping.getBuildTrigger().empty())
		{
			builderTrigger = monumentsMapping.getBuildTrigger();
			if (monumentsMapping.isOfBuilderCulture())
				builderTrigger += "AND = {\n\t\t\t\tculture = " + wonder->second->getBuilderCulture() + "\n\t\t\t\thas_owner_culture = yes\n\t\t\t}\n\t\t";
			if (monumentsMapping.isOfBuilderReligion())
				builderTrigger += "AND = {\n\t\t\t\treligion = " + wonder->second->getBuilderReligion() + "\n\t\t\t\thas_owner_religion = yes\n\t\t\t}\n\t\t";
		}
		for (const auto& mod: monumentsMapping.getProvinceModifiers())
			if (!provinceModifiers.count(mod.first))
			{
				provinceModifiers.emplace(mod);
				addedMod = true;
			}
		for (const auto& mod: monumentsMapping.getAreaModifiers())
			if (!provinceModifiers.count(mod.first))
			{
				areaModifiers.emplace(mod);
				addedMod = true;
			}
		for (const auto& mod: monumentsMapping.getCountryModifiers())
			if (!provinceModifiers.count(mod.first))
			{
				countryModifiers.emplace(mod);
				addedMod = true;
			}
		if (addedMod)
		{
			onUpgraded.emplace_back(monumentsMapping.getOnUpgraded()); //This way we will have 4 onUpgrades to match the 4 tiers
			numOfModifiers++;
		}
		if (numOfModifiers > 3)
			break;
	}
	

	
	output << wonder->second->getType() << "_" << wonder->second->getWonderID() << " = {\n\t";
	//Base
		output << "start = " << wonder->first << "\n\n\t";
		output << "date = " << wonder->second->getTrueDate() << "\n\n\t";
		output << "time = {\n\t\t months = 0\n\t}\n\n\t";
		output << "build_cost = 0\n\n\t";
		output << "can_be_moved = " << canBeMoved << "\n\n\t";
		output << "move_days_per_unit_distance = 2\n\n\t";
		output << "starting_tier = " << wonder->second->getStage() <<"\n\n\t";
		output << "type = monument\n\n\t";
		output << "build_trigger = {\n\t\t" << builderTrigger << "\n\t}\n\n\t";
		output << "on_built = {\n\t\t\n\t}\n\n\t";
		output << "on_destroyed = {\n\t\t\n\t}\n\n\t";
		output << "can_use_modifiers_trigger = {\n\t\t" << builderTrigger << "\n\t}\n\n\t";
		output << "can_upgrade_trigger = {\n\t\t" << builderTrigger << "\n\t}\n\n\t";
		output << "keep_trigger = {\n\t\t\n\t}\n\n\t";
	//Tier 0
		output << "tier_0 = {\n\t\t";
		output << "upgrade_time = {\n\t\t\tmonths = 0\n\t\t}\n\n\t";
		output << "cost_to_upgrade = {\n\t\t\tfactor = 0\n\t\t}\n\n\t";
		output << "province_modifiers = {\n\t\t\t";
			for (auto modifiers: provinceModifiers)
				output << modifiers.first << " = " << modifiers.second[0] << "\n\t\t\t";
			output << "\n\t\t}\n\n\t\t";
		output << "area_modifier = {\n\t\t\t";
			for (auto modifiers: areaModifiers)
				output << modifiers.first << " = " << modifiers.second[0] << "\n\t\t\t";
			output << "\n\t\t}\n\n\t\t";
		output << "country_modifiers = {\n\t\t\t";
			for (auto modifiers: countryModifiers)
				output << modifiers.first << " = " << modifiers.second[0] << "\n\t\t\t";
			output << "\n\t\t}\n\n\t\t";
		output << "on_upgraded = {\n\t\t\t";
			for (auto effect: onUpgraded)
				output << effect << "\n\t\t\t";
			output << "\n\t\t}";
		output << "\n\t}\n\n\t";
	//Tier 1
		output << "tier_1 = {\n\t\t";
		output << "upgrade_time = {\n\t\t\tmonths = 120\n\t\t}\n\n\t";
		output << "cost_to_upgrade = {\n\t\t\tfactor = 1000\n\t\t}\n\n\t";
		output << "province_modifiers = {\n\t\t\t";
			for (auto modifiers: provinceModifiers)
				output << modifiers.first << " = " << modifiers.second[1] << "\n\t\t\t";
			output << "\n\t\t}\n\n\t\t";
		output << "area_modifier = {\n\t\t\t";
			for (auto modifiers: areaModifiers)
				output << modifiers.first << " = " << modifiers.second[1] << "\n\t\t\t";
			output << "\n\t\t}\n\n\t\t";
		output << "country_modifiers = {\n\t\t\t";
			for (auto modifiers: countryModifiers)
				output << modifiers.first << " = " << modifiers.second[1] << "\n\t\t\t";
			output << "\n\t\t}\n\n\t\t";
		output << "on_upgraded = {\n\t\t\t";
			for (auto effect: onUpgraded)
				output << effect << "\n\t\t\t";
			output << "\n\t\t}";
		output << "\n\t}\n\n\t";
	//Tier 2
		output << "tier_2 = {\n\t\t";
		output << "upgrade_time = {\n\t\t\tmonths = 240\n\t\t}\n\n\t";
		output << "cost_to_upgrade = {\n\t\t\tfactor = 3500\n\t\t}\n\n\t";
		output << "province_modifiers = {\n\t\t\t";
			for (auto modifiers: provinceModifiers)
				output << modifiers.first << " = " << modifiers.second[2] << "\n\t\t\t";
			output << "\n\t\t}\n\n\t\t";
		output << "area_modifier = {\n\t\t\t";
			for (auto modifiers: areaModifiers)
				output << modifiers.first << " = " << modifiers.second[2] << "\n\t\t\t";
			output << "\n\t\t}\n\n\t\t";
		output << "country_modifiers = {\n\t\t\t";
			for (auto modifiers: countryModifiers)
				output << modifiers.first << " = " << modifiers.second[2] << "\n\t\t\t";
			output << "\n\t\t}\n\n\t\t";
		output << "on_upgraded = {\n\t\t\t";
			for (auto effect: onUpgraded)
				output << effect << "\n\t\t\t";
			output << "\n\t\t}";
		output << "\n\t}\n\n\t";
	//Tier 3
		output << "tier_3 = {\n\t\t";
		output << "upgrade_time = {\n\t\t\tmonths = 480\n\t\t}\n\n\t";
		output << "cost_to_upgrade = {\n\t\t\tfactor = 7000\n\t\t}\n\n\t";
		output << "province_modifiers = {\n\t\t\t";
			for (auto modifiers: provinceModifiers)
				output << modifiers.first << " = " << modifiers.second[0] << "\n\t\t\t";
			output << "\n\t\t}\n\n\t\t";
		output << "area_modifier = {\n\t\t\t";
			for (auto modifiers: areaModifiers)
				output << modifiers.first << " = " << modifiers.second[0] << "\n\t\t\t";
			output << "\n\t\t}\n\n\t\t";
		output << "country_modifiers = {\n\t\t\t";
			for (auto modifiers: countryModifiers)
				output << modifiers.first << " = " << modifiers.second[0] << "\n\t\t\t";
			output << "\n\t\t}\n\n\t\t";
		output << "on_upgraded = {\n\t\t\t";
			for (auto effect: onUpgraded)
				output << effect << "\n\t\t\t";
			output << "\n\t\t}";
		output << "\n\t}";
	output << "\n}";
}

EU4::outMonument::outMonument(const Configuration& theConfiguration, const std::set<std::string>& premades)
{
	if (premades.contains("wonder_pyramid_giza"))
		commonItems::TryCopyFile("configurables/monuments/great_projects/103_pyramids_of_cheops.txt", "output/" + theConfiguration.getOutputName() + "/common/great_projects/");
	else if (premades.contains("wonder_pagan_stones_stonehenge"))
		commonItems::TryCopyFile("configurables/monuments/great_projects/104_stonehenge.txt", "output/" + theConfiguration.getOutputName() + "/common/great_projects/");
	else if (premades.contains("wonder_mausoleum_halicarnassus"))
		commonItems::TryCopyFile("configurables/monuments/great_projects/105_mausoleum_at_helicarnassus.txt", "output/" + theConfiguration.getOutputName() + "/common/great_projects/");
	else if (premades.contains("wonder_lighthouse_alexandria"))
		commonItems::TryCopyFile("configurables/monuments/great_projects/106_lighthouse_of_alexandria.txt", "output/" + theConfiguration.getOutputName() + "/common/great_projects/");
	else if (premades.contains("wonder_temple_hindu_konark"))
		commonItems::TryCopyFile("configurables/monuments/great_projects/107_hindu_konark.txt", "output/" + theConfiguration.getOutputName() + "/common/great_projects/");
	else if (premades.contains("wonder_apostolic_palace"))
		commonItems::TryCopyFile("configurables/monuments/great_projects/108_apostolic_palace.txt", "output/" + theConfiguration.getOutputName() + "/common/great_projects/");
	else if (premades.contains("wonder_house_of_wisdom"))
		commonItems::TryCopyFile("configurables/monuments/great_projects/109_house_of_wisdom.txt", "output/" + theConfiguration.getOutputName() + "/common/great_projects/");
	else if (premades.contains("wonder_underground_city_petra"))
		commonItems::TryCopyFile("configurables/monuments/great_projects/110_petra.txt", "output/" + theConfiguration.getOutputName() + "/common/great_projects/");
	else if (premades.contains("wonder_cathedral_hagia_sophia"))
		commonItems::TryCopyFile("configurables/monuments/great_projects/111_hagia_sophia.txt", "output/" + theConfiguration.getOutputName() + "/common/great_projects/");
	else if (premades.contains("wonder_cathedral_notre_dame"))
		commonItems::TryCopyFile("configurables/monuments/great_projects/112_notre_dames.txt", "output/" + theConfiguration.getOutputName() + "/common/great_projects/");
}