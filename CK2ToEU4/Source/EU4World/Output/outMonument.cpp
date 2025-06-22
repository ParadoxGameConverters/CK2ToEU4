#include "outMonument.h"
#include <fstream>

EU4::outMonument::outMonument(const Configuration& theConfiguration, std::optional<std::pair<int, std::shared_ptr<CK2::Wonder>>> wonder, const int eu4Province)
{
	if (!wonder->second)
	{
		Log(LogLevel::Error) << "You fed me a dangling pointer.";
		return;
	}

	if (!wonder->second->hasBase() || (wonder->second->getProvinceModifiers().empty() && wonder->second->getAreaModifiers().empty() &&
													  wonder->second->getCountryModifiers().empty())) // This is a modded monument, probably better to not convert
		return;

	std::ofstream output("output" / theConfiguration.getOutputName() / "common/great_projects/!00_converted_monuments.txt", std::ios::out | std::ios::app);
	if (!output.is_open())
		throw std::runtime_error(
			 "Could not create monuments file: output/" + theConfiguration.getOutputName().string() + "/common/great_projects/!00_converted_monuments.txt");

	output << "\n#----------- " << wonder->second->getName() << " -----------\n";
	output << wonder->second->getType() << "_" << wonder->second->getWonderID() << " = {\n\t";
	// Base
	output << "start = " << eu4Province << "\n\n\t";
	output << "date = " << wonder->second->getTrueDate() << "\n\n\t";
	output << "time = {\n\t\t months = 0\n\t}\n\n\t";
	output << "build_cost = 0\n\n\t";
	output << "can_be_moved = " << wonder->second->getCanBeMoved() << "\n\n\t";
	output << "move_days_per_unit_distance = 2\n\n\t";
	output << "starting_tier = " << wonder->second->getStage() << "\n\n\t";
	output << "type = monument\n\n\t";
	output << "build_trigger = {\n\t\t" << wonder->second->getBuildTrigger() << "\n\t}\n\n\t";
	output << "on_built = {\n\t\t\n\t}\n\n\t";
	output << "on_destroyed = {\n\t\t\n\t}\n\n\t";
	output << "can_use_modifiers_trigger = {\n\t\t" << wonder->second->getBuildTrigger() << "\n\t}\n\n\t";
	output << "can_upgrade_trigger = {\n\t\t" << wonder->second->getBuildTrigger() << "\n\t}\n\n\t";
	output << "keep_trigger = {\n\t\t\n\t}\n\n\t";
	// Tier 0
	output << "tier_0 = {\n\t\t";
	output << "upgrade_time = {\n\t\t\tmonths = 0\n\t\t}\n\n\t\t";
	output << "cost_to_upgrade = {\n\t\t\tfactor = 0\n\t\t}\n\n\t\t";
	output << "province_modifiers = {\n\t\t\t";
	for (auto modifiers: wonder->second->getProvinceModifiers())
		output << modifiers.first << " = " << modifiers.second[0] << "\n\t\t\t";
	output << "\n\t\t}\n\n\t\t";
	output << "area_modifier = {\n\t\t\t";
	for (auto modifiers: wonder->second->getAreaModifiers())
		output << modifiers.first << " = " << modifiers.second[0] << "\n\t\t\t";
	output << "\n\t\t}\n\n\t\t";
	output << "country_modifiers = {\n\t\t\t";
	for (auto modifiers: wonder->second->getCountryModifiers())
		output << modifiers.first << " = " << modifiers.second[0] << "\n\t\t\t";
	output << "\n\t\t}\n\n\t\t";
	output << "on_upgraded = {\n\t\t\t";
	for (auto effect: wonder->second->getOnUpgraded())
		output << effect << "\n\t\t\t";
	output << "\n\t\t}";
	output << "\n\t}\n\n\t";
	// Tier 1
	output << "tier_1 = {\n\t\t";
	output << "upgrade_time = {\n\t\t\tmonths = 120\n\t\t}\n\n\t\t";
	output << "cost_to_upgrade = {\n\t\t\tfactor = 1000\n\t\t}\n\n\t\t";
	output << "province_modifiers = {\n\t\t\t";
	for (auto modifiers: wonder->second->getProvinceModifiers())
		output << modifiers.first << " = " << modifiers.second[1] << "\n\t\t\t";
	output << "\n\t\t}\n\n\t\t";
	output << "area_modifier = {\n\t\t\t";
	for (auto modifiers: wonder->second->getAreaModifiers())
		output << modifiers.first << " = " << modifiers.second[1] << "\n\t\t\t";
	output << "\n\t\t}\n\n\t\t";
	output << "country_modifiers = {\n\t\t\t";
	for (auto modifiers: wonder->second->getCountryModifiers())
		output << modifiers.first << " = " << modifiers.second[1] << "\n\t\t\t";
	output << "\n\t\t}\n\n\t\t";
	output << "on_upgraded = {\n\t\t\t";
	for (auto effect: wonder->second->getOnUpgraded())
		output << effect << "\n\t\t\t";
	output << "\n\t\t}";
	output << "\n\t}\n\n\t";
	// Tier 2
	output << "tier_2 = {\n\t\t";
	output << "upgrade_time = {\n\t\t\tmonths = 240\n\t\t}\n\n\t\t";
	output << "cost_to_upgrade = {\n\t\t\tfactor = 3500\n\t\t}\n\n\t\t";
	output << "province_modifiers = {\n\t\t\t";
	for (auto modifiers: wonder->second->getProvinceModifiers())
		output << modifiers.first << " = " << modifiers.second[2] << "\n\t\t\t";
	output << "\n\t\t}\n\n\t\t";
	output << "area_modifier = {\n\t\t\t";
	for (auto modifiers: wonder->second->getAreaModifiers())
		output << modifiers.first << " = " << modifiers.second[2] << "\n\t\t\t";
	output << "\n\t\t}\n\n\t\t";
	output << "country_modifiers = {\n\t\t\t";
	for (auto modifiers: wonder->second->getCountryModifiers())
		output << modifiers.first << " = " << modifiers.second[2] << "\n\t\t\t";
	output << "\n\t\t}\n\n\t\t";
	output << "on_upgraded = {\n\t\t\t";
	for (auto effect: wonder->second->getOnUpgraded())
		output << effect << "\n\t\t\t";
	output << "\n\t\t}";
	output << "\n\t}\n\n\t";
	// Tier 3
	output << "tier_3 = {\n\t\t";
	output << "upgrade_time = {\n\t\t\tmonths = 480\n\t\t}\n\n\t\t";
	output << "cost_to_upgrade = {\n\t\t\tfactor = 7000\n\t\t}\n\n\t\t";
	output << "province_modifiers = {\n\t\t\t";
	for (auto modifiers: wonder->second->getProvinceModifiers())
		output << modifiers.first << " = " << modifiers.second[3] << "\n\t\t\t";
	output << "\n\t\t}\n\n\t\t";
	output << "area_modifier = {\n\t\t\t";
	for (auto modifiers: wonder->second->getAreaModifiers())
		output << modifiers.first << " = " << modifiers.second[3] << "\n\t\t\t";
	output << "\n\t\t}\n\n\t\t";
	output << "country_modifiers = {\n\t\t\t";
	for (auto modifiers: wonder->second->getCountryModifiers())
		output << modifiers.first << " = " << modifiers.second[3] << "\n\t\t\t";
	output << "\n\t\t}\n\n\t\t";
	output << "on_upgraded = {\n\t\t\t";
	for (auto effect: wonder->second->getOnUpgraded())
		output << effect << "\n\t\t\t";
	output << "\n\t\t}";
	output << "\n\t}";
	output << "\n}\n\n#-----------------------------\n\n";

	output.close();

	// Now then, let's populate the GFX file
	std::ofstream gfxOutput("output" / theConfiguration.getOutputName() / "interface/zzz_converted_monuments.gfx", std::ios::out | std::ios::app);
	if (!gfxOutput.is_open())
		throw std::runtime_error(
			 "Could not create monuments file: output/" + theConfiguration.getOutputName().string() + "/interface/zzz_converted_monuments.gfx");

	gfxOutput << "spriteType = {\n\t\t";
	gfxOutput << "name = \"GFX_great_project_" << wonder->second->getType() << "_" << wonder->second->getWonderID() << "\"\n\t\t";
	gfxOutput << "texturefile = \"gfx//interface//great_projects//" << gfxType(wonder->second->getType()) << "\"\n\t";
	gfxOutput << "}\n\n\t";
	gfxOutput.close();

	// Finally, let's populate the localisation
	auto fileNames = commonItems::GetAllFilesInFolder("configurables/monuments/localisation/");
	for (const auto& fileName: fileNames)
	{
		std::ofstream locOutput("output" / theConfiguration.getOutputName() / "localisation" / fileName, std::ios::out | std::ios::app);
		locOutput << wonder->second->getType() << "_" << wonder->second->getWonderID() << ":0 \"" << wonder->second->getName() << "\"\n ";
		locOutput.close();
	}
}

EU4::outMonument::outMonument(const Configuration& theConfiguration, const std::set<std::string>& premades)
{
	if (premades.contains("wonder_pyramid_giza"))
		std::filesystem::copy_file("configurables/monuments/great_projects/103_pyramids_of_cheops.txt",
			 "output" / theConfiguration.getOutputName() / "common/great_projects/103_pyramids_of_cheops.txt");
	if (premades.contains("wonder_pagan_stones_stonehenge"))
		std::filesystem::copy_file("configurables/monuments/great_projects/104_stonehenge.txt",
			 "output" / theConfiguration.getOutputName() / "common/great_projects/104_stonehenge.txt");
	if (premades.contains("wonder_mausoleum_halicarnassus"))
		std::filesystem::copy_file("configurables/monuments/great_projects/105_mausoleum_at_helicarnassus.txt",
			 "output" / theConfiguration.getOutputName() / "common/great_projects/105_mausoleum_at_helicarnassus.txt");
	if (premades.contains("wonder_lighthouse_alexandria"))
		std::filesystem::copy_file("configurables/monuments/great_projects/106_lighthouse_of_alexandria.txt",
			 "output" / theConfiguration.getOutputName() / "common/great_projects/106_lighthouse_of_alexandria.txt");
	if (premades.contains("wonder_temple_hindu_konark"))
		std::filesystem::copy_file("configurables/monuments/great_projects/107_hindu_konark.txt",
			 "output" / theConfiguration.getOutputName() / "common/great_projects/107_hindu_konark.txt");
	if (premades.contains("wonder_apostolic_palace"))
		std::filesystem::copy_file("configurables/monuments/great_projects/108_apostolic_palace.txt",
			 "output" / theConfiguration.getOutputName() / "common/great_projects/108_apostolic_palace.txt");
	if (premades.contains("wonder_house_of_wisdom"))
		std::filesystem::copy_file("configurables/monuments/great_projects/109_house_of_wisdom.txt",
			 "output" / theConfiguration.getOutputName() / "common/great_projects/109_house_of_wisdom.txt");
	if (premades.contains("wonder_underground_city_petra"))
		std::filesystem::copy_file("configurables/monuments/great_projects/110_petra.txt",
			 "output" / theConfiguration.getOutputName() / "common/great_projects/110_petra.txt");
	if (premades.contains("wonder_cathedral_hagia_sophia"))
		std::filesystem::copy_file("configurables/monuments/great_projects/111_hagia_sophia.txt",
			 "output" / theConfiguration.getOutputName() / "common/great_projects/111_hagia_sophia.txt");
	if (premades.contains("wonder_cathedral_notre_dame"))
		std::filesystem::copy_file("configurables/monuments/great_projects/112_notre_dames.txt",
			 "output" / theConfiguration.getOutputName() / "common/great_projects/112_notre_dames.txt");
	// Wonders from Tianxia

	if (premades.contains("wonder_temple_cemetery_confucius"))
		std::filesystem::copy_file("configurables/monuments/great_projects/113_temple_of_confucius.txt", // I know that these two aren't the same monument, but
																																		 // they are extremely close and related to one another
			 "output" / theConfiguration.getOutputName() / "common/great_projects/113_temple_of_confucius.txt");
	if (premades.contains("wonder_shinto_grand_shrine_ise"))
		std::filesystem::copy_file("configurables/monuments/great_projects/114_jingu.txt",
			 "output" / theConfiguration.getOutputName() / "common/great_projects/114_jingu.txt");
	if (premades.contains("wonder_temple_buddhist_seokguram_bulguksa"))
		std::filesystem::copy_file("configurables/monuments/great_projects/115_bulguksa_temple.txt",
			 "output" / theConfiguration.getOutputName() / "common/great_projects/115_bulguksa_temple.txt");
	if (premades.contains("wonder_temple_hindu_angkor_wat"))
		std::filesystem::copy_file("configurables/monuments/great_projects/116_angkor_wat.txt",
			 "output" / theConfiguration.getOutputName() / "common/great_projects/116_angkor_wat.txt");
	if (premades.contains("wonder_temple_buddhist_borobudur"))
		std::filesystem::copy_file("configurables/monuments/great_projects/117_borobudur_temple.txt",
			 "output" / theConfiguration.getOutputName() / "common/great_projects/117.txt");
	if (premades.contains("wonder_temple_buddhist_shwedagon"))
		std::filesystem::copy_file("configurables/monuments/great_projects/118_shwedagon_temple.txt",
			 "output" / theConfiguration.getOutputName() / "common/great_projects/118_shwedagon_temple.txt");
}

const std::string EU4::outMonument::gfxType(const std::string& base)
{
	if (base == "wonder_cathedral")
		return "great_project_cologne_cathedral.dds";
	else if (base == "wonder_mosque")
		return "great_project_taj_mahal.dds";
	else if (base == "wonder_synagogue")
		return "great_project_kiev_pechersk_lavra.dds"; // Should be replaced as soon as a good replacement can be found
	else if (base == "wonder_temple_pagan")
		return "great_project_heddal_stave_church.dds";
	else if (base == "wonder_temple_buddhist")
		return "great_project_borobudur_temple.dds";
	else if (base == "wonder_temple_hindu")
		return "great_project_prambanan_temple.dds";
	else if (base == "wonder_statue_ruler")
		return "great_project_gomateshwara_statue.dds";
	else if (base == "wonder_statue_horse")
		return "great_project_easter_island_statues.dds"; // Should be replaced as soon as a good replacement can be found
	else if (base == "wonder_fortress")
		return "great_project_alhambra.dds";
	else if (base == "wonder_underground_city")
		return "great_project_petra.dds";
	else if (base == "wonder_wall")
		return "great_project_the_great_wall_of_china.dds";
	else if (base == "wonder_harbor")
		return "great_project_doges_palace.dds";
	else if (base == "wonder_lighthouse")
		return "great_project_belem_tower.dds"; // Should be replaced as soon as a good replacement can be found
	else if (base == "wonder_amphitheater")
		return "great_project_cartagena_de_indias.dds"; // Should be replaced as soon as a good replacement can be found
	else if (base == "wonder_palace")
		return "great_project_the_grand_palace.dds";
	else if (base == "wonder_garden")
		return "great_project_ambras_castle.dds"; // Should be replaced as soon as a good replacement can be found
	else if (base == "wonder_university")
		return "great_project_sankore_madrasah.dds";
	else if (base == "wonder_library")
		return "great_project_temple_of_confucius.dds"; // Should be replaced as soon as a good replacement can be found
	else if (base == "wonder_mausoleum")
		return "great_project_mausoleum_at_halicarnassus.dds";
	else if (base == "wonder_pyramid")
		return "great_project_pyramid_of_cheops.dds";
	else if (base == "great_project_stone_henge")
		return "great_project_pyramid_of_cheops.dds";
	else if (base == "wonder_aztec_pyramid")
		return "great_project_chichen_itza.dds";

	// Generic from Tianxia
	else if (base == "wonder_shinto_grand_shrine")
		return "great_project_sankin_kotai_palaces.dds"; // Possibly a former location of Hie Shrine
	else if (base == "wonder_temple_taoist")
		return "great_project_erdene_zuu.dds"; // Should be replaced as soon as a good replacement can be found
	else if (base == "wonder_temple_confucian")
		return "great_project_temple_of_confucius.dds";


	return "great_project_ait_benhaddou.dds"; // Fallback, just in case
}