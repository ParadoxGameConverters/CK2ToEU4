#include "../../CK2World/Titles/Title.h"
#include "../../Configuration/Configuration.h"
#include "../EU4World.h"
#include "Log.h"
#include "OSCompatibilityLayer.h"
#include "outCountry.h"
#include "outMonument.h"
#include "outReligion.h"
#include <filesystem>
#include <fstream>
namespace fs = std::filesystem;

void EU4::World::output(const commonItems::ConverterVersion& converterVersion, const Configuration& theConfiguration, const CK2::World& sourceWorld) const
{
	const auto isLeviathanDLCPresent = sourceWorld.isLeviathanDLCPresent();
	const auto invasion = sourceWorld.isInvasion();
	const auto dynamicInstitutions = theConfiguration.getDynamicInstitutions() == Configuration::INSTITUTIONS::DYNAMIC;
	const date conversionDate = sourceWorld.getConversionDate();
	Log(LogLevel::Info) << "<- Creating Output Folder";

	std::filesystem::create_directory("output");
	if (commonItems::DoesFolderExist("output" / theConfiguration.getOutputName()))
	{
		Log(LogLevel::Info) << "<< Deleting existing mod folder.";
		std::filesystem::remove_all("output" / theConfiguration.getOutputName());
	}
	Log(LogLevel::Progress) << "80 %";

	Log(LogLevel::Info) << "<- Copying Mod Template";
	std::filesystem::copy("blankMod/output", "output/output", std::filesystem::copy_options::recursive);
	Log(LogLevel::Progress) << "81 %";

	Log(LogLevel::Info) << "<- Moving Mod Template >> " << theConfiguration.getOutputName();
	std::filesystem::rename("output/output", "output" / theConfiguration.getOutputName());
	Log(LogLevel::Progress) << "82 %";

	std::filesystem::create_directory("output" / theConfiguration.getOutputName() / "history");
	std::filesystem::create_directory("output" / theConfiguration.getOutputName() / "history/countries/");
	std::filesystem::create_directory("output" / theConfiguration.getOutputName() / "history/advisors/");
	std::filesystem::create_directory("output" / theConfiguration.getOutputName() / "history/provinces/");
	std::filesystem::create_directory("output" / theConfiguration.getOutputName() / "history/diplomacy/");
	std::filesystem::create_directory("output" / theConfiguration.getOutputName() / "common/");
	std::filesystem::create_directory("output" / theConfiguration.getOutputName() / "common/countries/");
	std::filesystem::create_directory("output" / theConfiguration.getOutputName() / "common/country_tags/");
	std::filesystem::create_directory("output" / theConfiguration.getOutputName() / "localisation/");
	Log(LogLevel::Progress) << "83 %";

	Log(LogLevel::Info) << "<- Crafting .mod File";
	createModFile(theConfiguration);
	Log(LogLevel::Progress) << "84 %";

	// Record converter version
	Log(LogLevel::Info) << "<- Writing version";
	outputVersion(converterVersion, theConfiguration);
	Log(LogLevel::Progress) << "85 %";

	// Output common\countries.txt
	Log(LogLevel::Info) << "<- Creating countries.txt";
	outputCommonCountriesFile(theConfiguration);
	Log(LogLevel::Progress) << "86 %";

	Log(LogLevel::Info) << "<- Writing Country Commons";
	outputCommonCountries(theConfiguration);
	Log(LogLevel::Progress) << "87 %";

	Log(LogLevel::Info) << "<- Writing Country Histories";
	outputHistoryCountries(theConfiguration);
	Log(LogLevel::Progress) << "88 %";

	if (invasion)
	{
		Log(LogLevel::Info) << "<- Writing Sunset Invasion Files";
		outputInvasionExtras(theConfiguration);
	}
	if (dynamicInstitutions)
	{
		Log(LogLevel::Info) << "<- Writing Dynamic Institution Files";
		outputDynamicInstitutions(theConfiguration);
	}

	Log(LogLevel::Info) << "<- Writing Advisers";
	outputAdvisers(theConfiguration);
	Log(LogLevel::Progress) << "89 %";

	Log(LogLevel::Info) << "<- Writing Provinces";
	outputHistoryProvinces(theConfiguration, sourceWorld.getExistentPremadeMonuments(), isLeviathanDLCPresent);
	Log(LogLevel::Progress) << "90 %";

	Log(LogLevel::Info) << "<- Writing Localization";
	outputLocalization(theConfiguration, invasion, sourceWorld.isGreekReformation());
	Log(LogLevel::Progress) << "91 %";

	Log(LogLevel::Info) << "<- Writing Emperor";
	outputEmperor(theConfiguration, conversionDate);
	Log(LogLevel::Progress) << "92 %";

	Log(LogLevel::Info) << "<- Writing Diplomacy";
	outputDiplomacy(theConfiguration, diplomacy.getAgreements(), invasion);
	Log(LogLevel::Progress) << "93 %";

	Log(LogLevel::Info) << "<- Moving Flags";
	outputFlags(theConfiguration, sourceWorld);
	Log(LogLevel::Progress) << "94 %";

	Log(LogLevel::Info) << "<- Replacing Bookmark";
	outputBookmark(theConfiguration, conversionDate);
	Log(LogLevel::Progress) << "95 %";

	Log(LogLevel::Info) << "<- Writing Any Pagan Reformations";
	outputReformedReligions(theConfiguration, sourceWorld.wasNoReformation(), sourceWorld.getUnreligionReforms(), sourceWorld.getReligionReforms());
	Log(LogLevel::Progress) << "96 %";
}

void EU4::World::outputAdvisers(const Configuration& theConfiguration) const
{
	std::ofstream output("output" / theConfiguration.getOutputName() / "history/advisors/00_converter_advisors.txt");
	if (!output.is_open())
		throw std::runtime_error("Could not create " + theConfiguration.getOutputName().string() + "/history/advisors/00_converter_advisors.txt");
	for (const auto& country: countries)
	{
		country.second->outputAdvisers(output);
	}
	output.close();
}

void EU4::World::outputBookmark(const Configuration& theConfiguration, date conversionDate) const
{
	if (theConfiguration.getStartDateOption() == Configuration::STARTDATE::CK)
	{
		// fix the dynamic bookmark in defines
		if (!commonItems::DoesFileExist("output" / theConfiguration.getOutputName() / "common/defines/00_converter_defines.lua"))
			throw std::runtime_error("Can not find output/" + theConfiguration.getOutputName().string() + "/common/defines/00_converter_defines.lua!");
		std::ofstream defines("output" / theConfiguration.getOutputName() / "common/defines/00_converter_defines.lua");
		defines << "-- Defines modified by the converter\n\n";
		defines << "\nNDefines.NGame.START_DATE = \"" << conversionDate << "\"\n";
		defines.close();
		if (!commonItems::DoesFileExist("output" / theConfiguration.getOutputName() / "common/bookmarks/converter_bookmark.txt"))
			throw std::runtime_error("Can not find output/" + theConfiguration.getOutputName().string() + "/common/bookmarks/converter_bookmark.txt!");

		std::string startDate = "<CONVERSIONDATE>";
		std::ostringstream incomingBookmarks;

		std::ifstream bookmarks_txt("output" / theConfiguration.getOutputName() / "common/bookmarks/converter_bookmark.txt");
		incomingBookmarks << bookmarks_txt.rdbuf();
		bookmarks_txt.close();
		auto strBookmarks = incomingBookmarks.str();
		auto pos2 = strBookmarks.find(startDate);
		strBookmarks.replace(pos2, startDate.length(), conversionDate.toString());
		std::ofstream out_bookmarks_txt("output" / theConfiguration.getOutputName() / "common/bookmarks/converter_bookmark.txt");
		out_bookmarks_txt << strBookmarks;
		out_bookmarks_txt.close();

		// And wipe regular one
		std::ifstream bookmarks1444_txt("output" / theConfiguration.getOutputName() / "common/bookmarks/converter_bookmark_1444.txt",
			 std::ofstream::out | std::ofstream::trunc);
		bookmarks1444_txt.close();
	}
	else
	{
		// Vanilla defines are fine, just wipe dynamic bookmark
		std::ifstream bookmarks_txt("output" / theConfiguration.getOutputName() / "common/bookmarks/converter_bookmark.txt",
			 std::ofstream::out | std::ofstream::trunc);
		bookmarks_txt.close();
	}
}

void EU4::World::outputFlags(const Configuration& theConfiguration, const CK2::World& sourceWorld) const
{
	const auto invasion = sourceWorld.isInvasion();
	// Make a flag source registry
	std::map<std::filesystem::path, std::set<std::filesystem::path>> sourceFlagSources; // filename/fullpath

	auto fileNames = commonItems::GetAllFilesInFolder(theConfiguration.getCK2Path() / "gfx/flags");
	for (const auto& file: fileNames)
		sourceFlagSources[file].insert(theConfiguration.getCK2Path() / "gfx/flags" / file);

	for (const auto& mod: sourceWorld.getMods())
	{
		if (commonItems::DoesFolderExist(mod.path / "gfx/flags"))
		{
			Log(LogLevel::Info) << "\t>> Found some flags over in [" << mod.name << "]: " << mod.path << "/gfx/flags/";
			fileNames = commonItems::GetAllFilesInFolder(mod.path / "gfx/flags");
			for (const auto& file: fileNames)
				sourceFlagSources[file].insert(mod.path / "gfx/flags" / file);
		}
	}

	for (const auto& country: countries)
	{
		// first check is for dynasty and override flags.
		if (country.second->getHasDynastyName() && country.second->getDynastyID() &&
			 commonItems::DoesFileExist(std::filesystem::path("configurables/dynastyflags/" + std::to_string(country.second->getDynastyID()) + ".tga")))
		{
			std::filesystem::copy_file(std::filesystem::path("configurables/dynastyflags/" + std::to_string(country.second->getDynastyID()) + ".tga"),
				 "output" / theConfiguration.getOutputName() / "gfx/flags" / (country.first + ".tga"));
			continue;
		}

		// Otherwise, do we need a flag at all?
		if (commonItems::DoesFileExist(theConfiguration.getEU4Path() / "gfx/flags" / (country.first + ".tga")))
			continue;
		if (commonItems::DoesFileExist("output" / theConfiguration.getOutputName() / "gfx/flags" / (country.first + ".tga")))
			continue;
		// We do.
		if (country.second->getTitle().first.empty())
			continue; // Probably vanilla nation.
		auto titleName = country.second->getTitle().first;
		std::filesystem::path fileName;
		if (sourceFlagSources.contains(titleName + ".tga"))			  // we have some sources for this
			fileName = *sourceFlagSources[titleName + ".tga"].begin(); // So use the first one.
		if (fileName.empty() && !country.second->getTitle().second->getBaseTitle().first.empty())
		{
			titleName = country.second->getTitle().second->getBaseTitle().first;
			if (sourceFlagSources.contains(titleName + ".tga")) // we have some sources for base title
				fileName = *sourceFlagSources[titleName + ".tga"].begin();
			if (fileName.empty() && !country.second->getTitle().second->getBaseTitle().second->getBaseTitle().first.empty())
			{
				titleName = country.second->getTitle().second->getBaseTitle().second->getBaseTitle().first;
				if (sourceFlagSources.contains(titleName + ".tga")) // we have some sources for base title base title
					fileName = *sourceFlagSources[titleName + ".tga"].begin();
			}
		}
		if (fileName.empty())
			Log(LogLevel::Warning) << "failed to locate flag for " << country.first << ": " << country.second->getTitle().first;
		else
			std::filesystem::copy_file(fileName, "output" / theConfiguration.getOutputName() / "gfx/flags" / (country.first + ".tga"));
	}
	if (invasion)
		std::filesystem::copy_file("configurables/sunset/gfx/flags/SDM.tga", "output" / theConfiguration.getOutputName() / "gfx/flags/SDM.tga");
}

void EU4::World::createModFile(const Configuration& theConfiguration) const
{
	std::ofstream output("output" / std::filesystem::path(theConfiguration.getOutputName().string() + ".mod"));
	if (!output.is_open())
		throw std::runtime_error("Could not create " + theConfiguration.getOutputName().string() + ".mod");
	Log(LogLevel::Info) << "<< Writing to: "
							  << "output/" + theConfiguration.getOutputName().string() + ".mod";
	output << modFile;
	output.close();

	std::ofstream output2("output" / theConfiguration.getOutputName() / "descriptor.mod");
	if (!output2.is_open())
		throw std::runtime_error("Could not create " + theConfiguration.getOutputName().string() + "/descriptor.mod");
	Log(LogLevel::Info) << "<< Writing to: "
							  << "output/" + theConfiguration.getOutputName().string() + "/descriptor.mod";
	output2 << modFile;
	output2.close();
}


void EU4::World::outputLocalization(const Configuration& theConfiguration, bool invasion, bool greekReformation) const
{
	std::ofstream english("output" / theConfiguration.getOutputName() / "localisation/replace/converter_l_english.yml");
	std::ofstream french("output" / theConfiguration.getOutputName() / "localisation/replace/converter_l_french.yml");
	std::ofstream spanish("output" / theConfiguration.getOutputName() / "localisation/replace/converter_l_spanish.yml");
	std::ofstream german("output" / theConfiguration.getOutputName() / "localisation/replace/converter_l_german.yml");
	if (!english.is_open())
		throw std::runtime_error("Error writing localisation file! Is the output folder writable?");
	if (!french.is_open())
		throw std::runtime_error("Error writing localisation file! Is the output folder writable?");
	if (!spanish.is_open())
		throw std::runtime_error("Error writing localisation file! Is the output folder writable?");
	if (!german.is_open())
		throw std::runtime_error("Error writing localisation file! Is the output folder writable?");
	english << "\xEF\xBB\xBFl_english:\n"; // write BOM
	french << "\xEF\xBB\xBFl_french:\n";	// write BOM
	spanish << "\xEF\xBB\xBFl_spanish:\n"; // write BOM
	german << "\xEF\xBB\xBFl_german:\n";	// write BOM

	for (const auto& country: countries)
	{
		for (const auto& locblock: country.second->getLocalizations())
		{
			english << " " << locblock.first << ": \"" << commonItems::convertWin1252ToUTF8(locblock.second.english) << "\"\n";
			french << " " << locblock.first << ": \"" << commonItems::convertWin1252ToUTF8(locblock.second.french) << "\"\n";
			spanish << " " << locblock.first << ": \"" << commonItems::convertWin1252ToUTF8(locblock.second.spanish) << "\"\n";
			german << " " << locblock.first << ": \"" << commonItems::convertWin1252ToUTF8(locblock.second.german) << "\"\n";
		}
	}
	english.close();
	french.close();
	spanish.close();
	german.close();

	if (invasion)
	{
		auto fileNames = commonItems::GetAllFilesInFolder(std::filesystem::path("configurables/sunset/localisation"));
		for (const auto& fileName: fileNames)
			std::filesystem::copy("configurables/sunset/localisation" / fileName, "output" / theConfiguration.getOutputName() / "localisation" / fileName);
	}

	if (greekReformation)
	{
		auto fileNames = commonItems::GetAllFilesInFolder(std::filesystem::path("configurables/reformation/greek/"));
		for (const auto& fileName: fileNames)
			std::filesystem::copy("configurables/reformation/greek" / fileName, "output" / theConfiguration.getOutputName() / "localisation" / fileName);
	}
	else
	{
		auto fileNames = commonItems::GetAllFilesInFolder(std::filesystem::path("configurables/reformation/roman/"));
		for (const auto& fileName: fileNames)
			std::filesystem::copy("configurables/reformation/roman" / fileName, "output" / theConfiguration.getOutputName() / "localisation" / fileName);
	}
}

void EU4::World::outputVersion(const commonItems::ConverterVersion& converterVersion, const Configuration& theConfiguration) const
{
	std::ofstream output("output" / theConfiguration.getOutputName() / "ck2toeu4_version.txt");
	if (!output.is_open())
		throw std::runtime_error("Error writing version file! Is the output folder writable?");
	output << converterVersion;
	output.close();
}

void EU4::World::outputCommonCountriesFile(const Configuration& theConfiguration) const
{
	std::ofstream output("output" / theConfiguration.getOutputName() / "common/country_tags/00_countries.txt");
	if (!output.is_open())
		throw std::runtime_error("Could not create countries file!");
	output << "REB = \"countries/Rebels.txt\"\n\n"; // opening with rebels manually.

	for (const auto& country: countries)
	{
		if (specialCountryTags.count(country.first))
			continue; // Not outputting specials.
		if (country.first != "REB")
			output << country.first << " = \"countries/" << country.second->getCommonCountryFile().string() << "\"\n";
	}
	output << "\n";
	output.close();
}

void EU4::World::outputHistoryProvinces(const Configuration& theConfiguration, const std::set<std::string>& premades, const bool& isLeviathanDLCPresent) const
{
	if (isLeviathanDLCPresent)
	{
		std::filesystem::create_directory("output" / theConfiguration.getOutputName() / "common/great_projects");
		std::filesystem::copy_file("configurables/monuments/gfx/zzz_converted_monuments.gfx",
			 "output" / theConfiguration.getOutputName() / "interface/zzz_converted_monuments.gfx");
		auto fileNames = commonItems::GetAllFilesInFolder(std::filesystem::path("configurables/monuments/localisation"));
		for (const auto& fileName: fileNames)
			std::filesystem::copy_file("configurables/monuments/localisation" / fileName, "output" / theConfiguration.getOutputName() / "localisation" / fileName);
		outMonument(theConfiguration, premades); // Outputs Premade files
	}
	for (const auto& province: provinces)
	{
		std::ofstream output("output" / theConfiguration.getOutputName() / province.second->getHistoryCountryFile());
		if (!output.is_open())
			throw std::runtime_error("Could not create country history file: output/" + theConfiguration.getOutputName().string() + "/" +
											 province.second->getHistoryCountryFile().string());
		output << *province.second;
		output.close();

		if (province.second->getHasMonument() && !premades.contains(province.second->getSourceProvince()->getMonument()->second->getType()))
			outMonument(theConfiguration, province.second->getSourceProvince()->getMonument(), province.first);
	}
	if (isLeviathanDLCPresent) // This has to be last
	{
		// Final closing brace for the GFX file
		std::ofstream gfxOutput("output" / theConfiguration.getOutputName() / "interface/zzz_converted_monuments.gfx", std::ios::out | std::ios::app);
		if (!gfxOutput.is_open())
			throw std::runtime_error(
				 "Could not create monuments file: output/" + theConfiguration.getOutputName().string() + "/interface/zzz_converted_monuments.gfx");
		gfxOutput << "\n}";
		gfxOutput.close();
	}
}

void EU4::World::outputHistoryCountries(const Configuration& theConfiguration) const
{
	for (const auto& country: countries)
	{
		std::ofstream output("output" / theConfiguration.getOutputName() / "history/countries" / country.second->getHistoryCountryFile());
		if (!output.is_open())
			throw std::runtime_error("Could not create country history file: output/" + theConfiguration.getOutputName().string() + "/history/countries/" +
											 country.second->getHistoryCountryFile().string());
		output << *country.second;
		output.close();
	}
}

void EU4::World::outputCommonCountries(const Configuration& theConfiguration) const
{
	for (const auto& country: countries)
	{
		std::ofstream output("output" / theConfiguration.getOutputName() / "common/countries" / country.second->getCommonCountryFile());
		if (!output.is_open())
			throw std::runtime_error("Could not create country common file: output/" + theConfiguration.getOutputName().string() + "/common/countries/" +
											 country.second->getCommonCountryFile().string());
		country.second->outputCommons(output);
		output.close();
	}
}

void EU4::World::outputInvasionExtras(const Configuration& theConfiguration) const
{
	// Sunset Religions
	auto files = commonItems::GetAllFilesInFolder(std::filesystem::path("configurables/sunset/common/religions"));
	for (const auto& file: files)
		std::filesystem::copy_file("configurables/sunset/common/religions" / file, "output" / theConfiguration.getOutputName() / "common/religions" / file);
	// Sunset Ideas
	files = commonItems::GetAllFilesInFolder(std::filesystem::path("configurables/sunset/common/ideas/"));
	for (const auto& file: files)
		std::filesystem::copy_file("configurables/sunset/common/ideas" / file, "output" / theConfiguration.getOutputName() / "common/ideas" / file);
	// Sunset Cultures
	files = commonItems::GetAllFilesInFolder(std::filesystem::path("configurables/sunset/common/cultures/"));
	for (const auto& file: files)
		std::filesystem::copy_file("configurables/sunset/common/cultures" / file, "output" / theConfiguration.getOutputName() / "common/cultures" / file);
	// Sunset Decisions
	files = commonItems::GetAllFilesInFolder(std::filesystem::path("configurables/sunset/decisions/"));
	for (const auto& file: files)
		std::filesystem::copy_file("configurables/sunset/decisions" / file, "output" / theConfiguration.getOutputName() / "decisions" / file);
}
void EU4::World::outputDynamicInstitutions(const Configuration& theConfiguration) const
{
	// Dynamic Institions
	auto files = commonItems::GetAllFilesInFolder(std::filesystem::path("configurables/dynamicInstitutions/institutions"));
	std::filesystem::create_directory("output" / theConfiguration.getOutputName() / "common/institutions");
	for (const auto& file: files)
		std::filesystem::copy_file("configurables/dynamicInstitutions/institutions" / file,
			 "output" / theConfiguration.getOutputName() / "common/institutions" / file);
	// Dynamic Ideas
	files = commonItems::GetAllFilesInFolder(std::filesystem::path("configurables/dynamicInstitutions/ideas"));
	for (const auto& file: files)
		std::filesystem::copy_file("configurables/dynamicInstitutions/ideas" / file, "output" / theConfiguration.getOutputName() / "common/ideas" / file);
}

void EU4::World::outputEmperor(const Configuration& theConfiguration, date conversionDate) const
{
	auto actualConversionDate = conversionDate;
	if (theConfiguration.getStartDateOption() == Configuration::STARTDATE::EU)
		actualConversionDate = date(1444, 11, 11);

	std::ofstream output("output" / theConfiguration.getOutputName() / "history/diplomacy/hre.txt");
	if (!output.is_open())
		throw std::runtime_error("Could not create hre diplomacy file: output/" + theConfiguration.getOutputName().string() + "/history/diplomacy/hre.txt!");
	if (emperorTag.empty())
		output << actualConversionDate << " = { emperor = --- }\n";
	else
		output << actualConversionDate << " = { emperor = " << emperorTag << " }\n";
	output.close();

	output.open("output" / theConfiguration.getOutputName() / "history/diplomacy/celestial_empire.txt");
	if (!output.is_open())
		throw std::runtime_error(
			 "Could not create celestial empire diplomacy file: output/" + theConfiguration.getOutputName().string() + "/history/diplomacy/celestial_empire.txt!");
	if (celestialEmperorTag.empty())
		output << actualConversionDate << " = { celestial_emperor = --- }\n";
	else
		output << actualConversionDate << " = { celestial_emperor = " << celestialEmperorTag << " }\n";
	output.close();

	if (!actualHRETag.empty())
	{
		output.open("output" / theConfiguration.getOutputName() / "i_am_hre.txt");
		output << actualHRETag;
		output.close();

		if (actualHRETag != "HRE")
		{
			std::ifstream input(theConfiguration.getEU4Path() / "events/HolyRomanEmpire.txt");
			if (!input.is_open())
			{
				Log(LogLevel::Warning) << "Where is " << theConfiguration.getEU4Path().string() << "/events/HolyRomanEmpire.txt?!";
			}
			else
			{
				std::stringstream inStream;
				inStream << input.rdbuf();
				auto eventFileString = inStream.str();
				input.close();
				eventFileString = std::regex_replace(eventFileString, std::regex("HLR"), actualHRETag);
				output.open("output" / theConfiguration.getOutputName() / "events/HolyRomanEmpire.txt");
				output << eventFileString;
				output.close();
			}
		}
	}
}

void EU4::World::outputDiplomacy(const Configuration& theConfiguration, const std::vector<std::shared_ptr<Agreement>>& agreements, bool invasion) const
{
	std::ofstream alliances("output" / theConfiguration.getOutputName() / "history/diplomacy/converter_alliances.txt");
	if (!alliances.is_open())
		throw std::runtime_error("Could not create alliances history file!");

	std::ofstream guarantees("output" / theConfiguration.getOutputName() / "history/diplomacy/converter_guarantees.txt");
	if (!guarantees.is_open())
		throw std::runtime_error("Could not create guarantees history file!");

	std::ofstream puppetStates("output" / theConfiguration.getOutputName() / "history/diplomacy/converter_puppetstates.txt");
	if (!puppetStates.is_open())
		throw std::runtime_error("Could not create puppet states history file!");

	std::ofstream unions("output/" / theConfiguration.getOutputName() / "history/diplomacy/converter_unions.txt");
	if (!unions.is_open())
		throw std::runtime_error("Could not create unions history file!");

	for (const auto& agreement: agreements)
	{
		if (agreement->getType() == "guarantee")
		{
			guarantees << *agreement;
		}
		else if (agreement->getType() == "union")
		{
			unions << *agreement;
		}
		else if (agreement->getType() == "vassal" || agreement->getType() == "dependency")
		{
			puppetStates << *agreement;
		}
		else if (agreement->getType() == "alliance")
		{
			alliances << *agreement;
		}
		else
		{
			Log(LogLevel::Warning) << "Cannot output diplomatic agreement type " << agreement->getType() << "!";
		}
	}

	alliances.close();
	guarantees.close();
	puppetStates.close();
	unions.close();

	if (invasion)
	{
		// Blank american diplomacy
		std::ofstream diplo("output" / theConfiguration.getOutputName() / "history/diplomacy/American_alliances.txt");
		diplo << "\n";
		diplo.close();
		// and move over our alliances.
		std::filesystem::copy_file("configurables/sunset/history/diplomacy/SunsetInvasion.txt",
			 "output" / theConfiguration.getOutputName() / "history/diplomacy/SunsetInvasion.txt");
	}
}

void EU4::World::outputReformedReligions(const Configuration& theConfiguration,
	 bool noReformation,
	 const std::vector<mappers::ReformedReligionMapping>& unreligionReforms,
	 const std::vector<mappers::ReformedReligionMapping>& religionReforms) const
{
	if (noReformation)
	{
		auto files = commonItems::GetAllFilesInFolder(std::filesystem::path("configurables/reformation/oldPagans/"));
		for (const auto& file: files)
			std::filesystem::copy_file("configurables/reformation/oldPagans" / file, "output" / theConfiguration.getOutputName() / "common/religions" / file);
	}
	else
	{
		outReligion(theConfiguration, unreligionReforms, religionReforms);
	}
}