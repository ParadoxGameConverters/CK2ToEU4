#include "../EU4World.h"
#include "Log.h"
#include <filesystem>
#include <fstream>
namespace fs = std::filesystem;
#include "../../CK2World/Titles/Title.h"
#include "../../Configuration/Configuration.h"
#include "OSCompatibilityLayer.h"
#include "outCountry.h"
#include "outReligion.h"

void EU4::World::output(const mappers::VersionParser& versionParser, const Configuration& theConfiguration, const CK2::World& sourceWorld) const
{
	const auto invasion = sourceWorld.isInvasion();
	const date conversionDate = sourceWorld.getConversionDate();
	LOG(LogLevel::Info) << "<- Creating Output Folder";

	commonItems::TryCreateFolder("output");
	if (commonItems::DoesFolderExist("output/" + theConfiguration.getOutputName()))
	{
		Log(LogLevel::Info) << "<< Deleting existing mod folder.";
		commonItems::DeleteFolder("output/" + theConfiguration.getOutputName());
	}
	Log(LogLevel::Progress) << "80 %";

	LOG(LogLevel::Info) << "<- Copying Mod Template";
	commonItems::CopyFolder("blankMod/output", "output/output");
	Log(LogLevel::Progress) << "81 %";

	LOG(LogLevel::Info) << "<- Moving Mod Template >> " << theConfiguration.getOutputName();
	commonItems::RenameFolder("output/output", "output/" + theConfiguration.getOutputName());
	Log(LogLevel::Progress) << "82 %";

	commonItems::TryCreateFolder("output/" + theConfiguration.getOutputName() + "/history/");
	commonItems::TryCreateFolder("output/" + theConfiguration.getOutputName() + "/history/countries/");
	commonItems::TryCreateFolder("output/" + theConfiguration.getOutputName() + "/history/advisors/");
	commonItems::TryCreateFolder("output/" + theConfiguration.getOutputName() + "/history/provinces/");
	commonItems::TryCreateFolder("output/" + theConfiguration.getOutputName() + "/history/diplomacy/");
	commonItems::TryCreateFolder("output/" + theConfiguration.getOutputName() + "/common/");
	commonItems::TryCreateFolder("output/" + theConfiguration.getOutputName() + "/common/countries/");
	commonItems::TryCreateFolder("output/" + theConfiguration.getOutputName() + "/common/country_tags/");
	commonItems::TryCreateFolder("output/" + theConfiguration.getOutputName() + "/localisation/");
	Log(LogLevel::Progress) << "83 %";

	LOG(LogLevel::Info) << "<- Crafting .mod File";
	createModFile(theConfiguration);
	Log(LogLevel::Progress) << "84 %";

	// Record converter version
	LOG(LogLevel::Info) << "<- Writing version";
	outputVersion(versionParser, theConfiguration);
	Log(LogLevel::Progress) << "85 %";

	// Output common\countries.txt
	LOG(LogLevel::Info) << "<- Creating countries.txt";
	outputCommonCountriesFile(theConfiguration);
	Log(LogLevel::Progress) << "86 %";

	LOG(LogLevel::Info) << "<- Writing Country Commons";
	outputCommonCountries(theConfiguration);
	Log(LogLevel::Progress) << "87 %";

	LOG(LogLevel::Info) << "<- Writing Country Histories";
	outputHistoryCountries(theConfiguration);
	Log(LogLevel::Progress) << "88 %";

	if (invasion)
	{
		LOG(LogLevel::Info) << "<- Writing Sunset Invasion Files";
		outputInvasionExtras(theConfiguration, invasion);
	}

	LOG(LogLevel::Info) << "<- Writing Advisers";
	outputAdvisers(theConfiguration);
	Log(LogLevel::Progress) << "89 %";

	LOG(LogLevel::Info) << "<- Writing Provinces";
	outputHistoryProvinces(theConfiguration);
	Log(LogLevel::Progress) << "90 %";

	LOG(LogLevel::Info) << "<- Writing Localization";
	outputLocalization(theConfiguration, invasion, sourceWorld.isGreekReformation());
	Log(LogLevel::Progress) << "91 %";

	LOG(LogLevel::Info) << "<- Writing Emperor";
	outputEmperor(theConfiguration, conversionDate);
	Log(LogLevel::Progress) << "92 %";

	LOG(LogLevel::Info) << "<- Writing Diplomacy";
	outputDiplomacy(theConfiguration, diplomacy.getAgreements(), invasion);
	Log(LogLevel::Progress) << "93 %";

	LOG(LogLevel::Info) << "<- Moving Flags";
	outputFlags(theConfiguration, sourceWorld);
	Log(LogLevel::Progress) << "94 %";

	LOG(LogLevel::Info) << "<- Replacing Bookmark";
	outputBookmark(theConfiguration, conversionDate);
	Log(LogLevel::Progress) << "95 %";

	LOG(LogLevel::Info) << "<- Writing Any Pagan Reformations";
	outputReformedReligions(theConfiguration, sourceWorld.wasNoReformation(), sourceWorld.getUnreligionReforms(), sourceWorld.getReligionReforms());
	Log(LogLevel::Progress) << "96 %";
}

void EU4::World::outputAdvisers(const Configuration& theConfiguration) const
{
	std::ofstream output("output/" + theConfiguration.getOutputName() + "/history/advisors/00_converter_advisors.txt");
	if (!output.is_open())
		throw std::runtime_error("Could not create " + theConfiguration.getOutputName() + "/history/advisors/00_converter_advisors.txt");
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
		if (!commonItems::DoesFileExist("output/" + theConfiguration.getOutputName() + "/common/defines/00_converter_defines.lua"))
			throw std::runtime_error("Can not find output/" + theConfiguration.getOutputName() + "/common/defines/00_converter_defines.lua!");
		std::ofstream defines("output/" + theConfiguration.getOutputName() + "/common/defines/00_converter_defines.lua");
		defines << "-- Defines modified by the converter\n\n";
		defines << "\nNDefines.NGame.START_DATE = \"" << conversionDate << "\"\n";
		defines.close();
		if (!commonItems::DoesFileExist("output/" + theConfiguration.getOutputName() + "/common/bookmarks/converter_bookmark.txt"))
			throw std::runtime_error("Can not find output/" + theConfiguration.getOutputName() + "/common/bookmarks/converter_bookmark.txt!");

		std::string startDate = "<CONVERSIONDATE>";
		std::ostringstream incomingBookmarks;

		std::ifstream bookmarks_txt("output/" + theConfiguration.getOutputName() + "/common/bookmarks/converter_bookmark.txt");
		incomingBookmarks << bookmarks_txt.rdbuf();
		bookmarks_txt.close();
		auto strBookmarks = incomingBookmarks.str();
		auto pos2 = strBookmarks.find(startDate);
		strBookmarks.replace(pos2, startDate.length(), conversionDate.toString());
		std::ofstream out_bookmarks_txt("output/" + theConfiguration.getOutputName() + "/common/bookmarks/converter_bookmark.txt");
		out_bookmarks_txt << strBookmarks;
		out_bookmarks_txt.close();

		// And wipe regular one
		std::ifstream bookmarks1444_txt("output/" + theConfiguration.getOutputName() + "/common/bookmarks/converter_bookmark_1444.txt",
			 std::ofstream::out | std::ofstream::trunc);
		bookmarks1444_txt.close();
	}
	else
	{
		// Vanilla defines are fine, just wipe dynamic bookmark
		std::ifstream bookmarks_txt("output/" + theConfiguration.getOutputName() + "/common/bookmarks/converter_bookmark.txt",
			 std::ofstream::out | std::ofstream::trunc);
		bookmarks_txt.close();
	}
}

void EU4::World::outputFlags(const Configuration& theConfiguration, const CK2::World& sourceWorld) const
{
	const auto invasion = sourceWorld.isInvasion();
	// Make a flag source registry
	std::map<std::string, std::set<std::string>> sourceFlagSources; // filename/fullpath

	auto fileNames = commonItems::GetAllFilesInFolder(theConfiguration.getCK2Path() + "/gfx/flags/");
	for (const auto& file: fileNames)
		sourceFlagSources[file].insert(theConfiguration.getCK2Path() + "/gfx/flags/" + file);

	for (const auto& mod: sourceWorld.getMods().getMods())
	{
		if (commonItems::DoesFolderExist(mod.second + "/gfx/flags/"))
		{
			Log(LogLevel::Info) << "\t>> Found some flags over in: " << mod.second << "/gfx/flags/";
			fileNames = commonItems::GetAllFilesInFolder(mod.second + "/gfx/flags/");
			for (const auto& file: fileNames)
				sourceFlagSources[file].insert(mod.second + "/gfx/flags/" + file);
		}
	}

	for (const auto& country: countries)
	{
		// first check is for dynasty and override flags.
		if (country.second->getHasDynastyName() && country.second->getDynastyID() &&
			 commonItems::DoesFileExist("configurables/dynastyflags/" + std::to_string(country.second->getDynastyID()) + ".tga"))
		{
			commonItems::TryCopyFile("configurables/dynastyflags/" + std::to_string(country.second->getDynastyID()) + ".tga",
				 "output/" + theConfiguration.getOutputName() + "/gfx/flags/" + country.first + ".tga");
			continue;
		}

		// Otherwise, do we need a flag at all?
		if (commonItems::DoesFileExist(theConfiguration.getEU4Path() + "/gfx/flags/" + country.first + ".tga"))
			continue;
		if (commonItems::DoesFileExist("output/" + theConfiguration.getOutputName() + "/gfx/flags/" + country.first + ".tga"))
			continue;
		// We do.
		if (country.second->getTitle().first.empty())
			continue; // Probably vanilla nation.
		auto titleName = country.second->getTitle().first;
		std::string fileName;
		if (sourceFlagSources.count(titleName + ".tga"))				  // we have some sources for this
			fileName = *sourceFlagSources[titleName + ".tga"].begin(); // So use the first one.
		if (fileName.empty() && !country.second->getTitle().second->getBaseTitle().first.empty())
		{
			titleName = country.second->getTitle().second->getBaseTitle().first;
			if (sourceFlagSources.count(titleName + ".tga")) // we have some sources for base title
				fileName = *sourceFlagSources[titleName + ".tga"].begin();
			if (fileName.empty() && !country.second->getTitle().second->getBaseTitle().second->getBaseTitle().first.empty())
			{
				titleName = country.second->getTitle().second->getBaseTitle().second->getBaseTitle().first;
				if (sourceFlagSources.count(titleName + ".tga")) // we have some sources for base title base title
					fileName = *sourceFlagSources[titleName + ".tga"].begin();
			}
		}
		if (fileName.empty())
			Log(LogLevel::Warning) << "failed to locate flag for " << country.first << ": " << country.second->getTitle().first;
		else
			commonItems::TryCopyFile(fileName, "output/" + theConfiguration.getOutputName() + "/gfx/flags/" + country.first + ".tga");
	}
	if (invasion)
		commonItems::TryCopyFile("configurables/sunset/gfx/flags/SDM.tga", "output/" + theConfiguration.getOutputName() + "/gfx/flags/SDM.tga");
}

void EU4::World::createModFile(const Configuration& theConfiguration) const
{
	std::ofstream output("output/" + theConfiguration.getOutputName() + ".mod");
	if (!output.is_open())
		throw std::runtime_error("Could not create " + theConfiguration.getOutputName() + ".mod");
	LOG(LogLevel::Info) << "<< Writing to: "
							  << "output/" + theConfiguration.getOutputName() + ".mod";
	output << modFile;
	output.close();

	std::ofstream output2("output/" + theConfiguration.getOutputName() + "/descriptor.mod");
	if (!output2.is_open())
		throw std::runtime_error("Could not create " + theConfiguration.getOutputName() + "/descriptor.mod");
	LOG(LogLevel::Info) << "<< Writing to: "
							  << "output/" + theConfiguration.getOutputName() + "/descriptor.mod";
	output2 << modFile;
	output2.close();
}


void EU4::World::outputLocalization(const Configuration& theConfiguration, bool invasion, bool greekReformation) const
{
	std::ofstream english("output/" + theConfiguration.getOutputName() + "/localisation/replace/converter_l_english.yml");
	std::ofstream french("output/" + theConfiguration.getOutputName() + "/localisation/replace/converter_l_french.yml");
	std::ofstream spanish("output/" + theConfiguration.getOutputName() + "/localisation/replace/converter_l_spanish.yml");
	std::ofstream german("output/" + theConfiguration.getOutputName() + "/localisation/replace/converter_l_german.yml");
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
		auto fileNames = commonItems::GetAllFilesInFolder("configurables/sunset/localisation/");
		for (const auto& fileName: fileNames)
			commonItems::TryCopyFile("configurables/sunset/localisation/" + fileName, "output/" + theConfiguration.getOutputName() + "/localisation/" + fileName);
	}

	if (greekReformation)
	{
		auto fileNames = commonItems::GetAllFilesInFolder("configurables/reformation/greek/");
		for (const auto& fileName: fileNames)
			commonItems::TryCopyFile("configurables/reformation/greek/" + fileName, "output/" + theConfiguration.getOutputName() + "/localisation/" + fileName);
	}
	else
	{
		auto fileNames = commonItems::GetAllFilesInFolder("configurables/reformation/roman/");
		for (const auto& fileName: fileNames)
			commonItems::TryCopyFile("configurables/reformation/roman/" + fileName, "output/" + theConfiguration.getOutputName() + "/localisation/" + fileName);
	}
}

void EU4::World::outputVersion(const mappers::VersionParser& versionParser, const Configuration& theConfiguration) const
{
	std::ofstream output("output/" + theConfiguration.getOutputName() + "/ck2toeu4_version.txt");
	if (!output.is_open())
		throw std::runtime_error("Error writing version file! Is the output folder writable?");
	output << versionParser;
	output.close();
}

void EU4::World::outputCommonCountriesFile(const Configuration& theConfiguration) const
{
	std::ofstream output("output/" + theConfiguration.getOutputName() + "/common/country_tags/00_countries.txt");
	if (!output.is_open())
		throw std::runtime_error("Could not create countries file!");
	output << "REB = \"countries/Rebels.txt\"\n\n"; // opening with rebels manually.

	for (const auto& country: countries)
	{
		if (specialCountryTags.count(country.first))
			continue; // Not outputting specials.
		if (country.first != "REB")
			output << country.first << " = \"" << country.second->getCommonCountryFile() << "\"\n";
	}
	output << "\n";
	output.close();
}

void EU4::World::outputHistoryProvinces(const Configuration& theConfiguration) const
{
	for (const auto& province: provinces)
	{
		std::ofstream output("output/" + theConfiguration.getOutputName() + "/" + province.second->getHistoryCountryFile());
		if (!output.is_open())
			throw std::runtime_error(
				 "Could not create country history file: output/" + theConfiguration.getOutputName() + "/" + province.second->getHistoryCountryFile());
		output << *province.second;
		output.close();
	}
}

void EU4::World::outputHistoryCountries(const Configuration& theConfiguration) const
{
	for (const auto& country: countries)
	{
		std::ofstream output("output/" + theConfiguration.getOutputName() + "/" + country.second->getHistoryCountryFile());
		if (!output.is_open())
			throw std::runtime_error(
				 "Could not create country history file: output/" + theConfiguration.getOutputName() + "/" + country.second->getHistoryCountryFile());
		output << *country.second;
		output.close();
	}
}

void EU4::World::outputCommonCountries(const Configuration& theConfiguration) const
{
	for (const auto& country: countries)
	{
		std::ofstream output("output/" + theConfiguration.getOutputName() + "/common/" + country.second->getCommonCountryFile());
		if (!output.is_open())
			throw std::runtime_error(
				 "Could not create country common file: output/" + theConfiguration.getOutputName() + "/common/" + country.second->getCommonCountryFile());
		country.second->outputCommons(output);
		output.close();
	}
}

void EU4::World::outputInvasionExtras(const Configuration& theConfiguration, bool invasion) const
{
	// Sunset Religions
	auto files = commonItems::GetAllFilesInFolder("configurables/sunset/common/religions/");
	for (const auto& file: files)
		commonItems::TryCopyFile("configurables/sunset/common/religions/" + file, "output/" + theConfiguration.getOutputName() + "/common/religions/" + file);
	// Sunset Ideas
	files = commonItems::GetAllFilesInFolder("configurables/sunset/common/ideas/");
	for (const auto& file: files)
		commonItems::TryCopyFile("configurables/sunset/common/ideas/" + file, "output/" + theConfiguration.getOutputName() + "/common/ideas/" + file);
	// Sunset Cultures
	files = commonItems::GetAllFilesInFolder("configurables/sunset/common/cultures/");
	for (const auto& file: files)
		commonItems::TryCopyFile("configurables/sunset/common/cultures/" + file, "output/" + theConfiguration.getOutputName() + "/common/cultures/" + file);
	// Sunset Decisions
	files = commonItems::GetAllFilesInFolder("configurables/sunset/decisions/");
	for (const auto& file: files)
		commonItems::TryCopyFile("configurables/sunset/decisions/" + file, "output/" + theConfiguration.getOutputName() + "/decisions/" + file);
}

void EU4::World::outputEmperor(const Configuration& theConfiguration, date conversionDate) const
{
	auto actualConversionDate = conversionDate;
	if (theConfiguration.getStartDateOption() == Configuration::STARTDATE::EU)
		actualConversionDate = date(1444, 11, 11);

	std::ofstream output("output/" + theConfiguration.getOutputName() + "/history/diplomacy/hre.txt");
	if (!output.is_open())
		throw std::runtime_error("Could not create hre diplomacy file: output/" + theConfiguration.getOutputName() + "/history/diplomacy/hre.txt!");
	if (emperorTag.empty())
		output << actualConversionDate << " = { emperor = --- }\n";
	else
		output << actualConversionDate << " = { emperor = " << emperorTag << " }\n";
	output.close();

	std::ofstream output2("output/" + theConfiguration.getOutputName() + "/history/diplomacy/celestial_empire.txt");
	if (!output2.is_open())
		throw std::runtime_error(
			 "Could not create celestial empire diplomacy file: output/" + theConfiguration.getOutputName() + "/history/diplomacy/celestial_empire.txt!");
	if (celestialEmperorTag.empty())
		output2 << actualConversionDate << " = { celestial_emperor = --- }\n";
	else
		output2 << actualConversionDate << " = { celestial_emperor = " << celestialEmperorTag << " }\n";
	output2.close();
}

void EU4::World::outputDiplomacy(const Configuration& theConfiguration, const std::vector<std::shared_ptr<Agreement>>& agreements, bool invasion) const
{
	std::ofstream alliances("output/" + theConfiguration.getOutputName() + "/history/diplomacy/converter_alliances.txt");
	if (!alliances.is_open())
		throw std::runtime_error("Could not create alliances history file!");

	std::ofstream guarantees("output/" + theConfiguration.getOutputName() + "/history/diplomacy/converter_guarantees.txt");
	if (!guarantees.is_open())
		throw std::runtime_error("Could not create guarantees history file!");

	std::ofstream puppetStates("output/" + theConfiguration.getOutputName() + "/history/diplomacy/converter_puppetstates.txt");
	if (!puppetStates.is_open())
		throw std::runtime_error("Could not create puppet states history file!");

	std::ofstream unions("output/" + theConfiguration.getOutputName() + "/history/diplomacy/converter_unions.txt");
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
			LOG(LogLevel::Warning) << "Cannot output diplomatic agreement type " << agreement->getType() << "!";
		}
	}

	alliances.close();
	guarantees.close();
	puppetStates.close();
	unions.close();

	if (invasion)
	{
		// Blank american diplomacy
		std::ofstream diplo("output/" + theConfiguration.getOutputName() + "/history/diplomacy/American_alliances.txt");
		diplo << "\n";
		diplo.close();
		// and move over our alliances.
		commonItems::TryCopyFile("configurables/sunset/history/diplomacy/SunsetInvasion.txt",
			 "output/" + theConfiguration.getOutputName() + "/history/diplomacy/SunsetInvasion.txt");
	}
}

void EU4::World::outputReformedReligions(const Configuration& theConfiguration,
	 bool noReformation,
	 const std::vector<mappers::ReformedReligionMapping>& unreligionReforms,
	 const std::vector<mappers::ReformedReligionMapping>& religionReforms) const
{
	if (noReformation)
	{
		auto files = commonItems::GetAllFilesInFolder("configurables/reformation/oldPagans/");
		for (const auto& file: files)
			commonItems::TryCopyFile("configurables/reformation/oldPagans/" + file, "output/" + theConfiguration.getOutputName() + "/common/religions/" + file);
	}
	else
	{
		outReligion(theConfiguration, unreligionReforms, religionReforms);
	}
}