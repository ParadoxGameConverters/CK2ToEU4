#include "../EU4World.h"
#include "Log.h"
#include <filesystem>
#include <fstream>
namespace fs = std::filesystem;
#include "../../Configuration/Configuration.h"
#include "OSCompatibilityLayer.h"
#include "../../CK2World/Titles/Title.h"

void EU4::World::output(const mappers::VersionParser& versionParser, const Configuration& theConfiguration) const
{
	LOG(LogLevel::Info) << "<- Creating Output Folder";
	fs::create_directory("output");
	if (fs::exists("output/" + theConfiguration.getOutputName())) {
		Log(LogLevel::Info) << "<< Deleting existing mod folder.";
		fs::remove_all("output/" + theConfiguration.getOutputName());
	}
	LOG(LogLevel::Info) << "<- Copying Mod Template";
	fs::copy("blankMod/output", "output/output", fs::copy_options::recursive);
	LOG(LogLevel::Info) << "<- Moving Mod Template >> " << theConfiguration.getOutputName();
	fs::rename("output/output", "output/" + theConfiguration.getOutputName());

	fs::create_directory("output/" + theConfiguration.getOutputName() + "/history/");
	fs::create_directory("output/" + theConfiguration.getOutputName() + "/history/countries/");
	fs::create_directory("output/" + theConfiguration.getOutputName() + "/history/provinces/");
	fs::create_directory("output/" + theConfiguration.getOutputName() + "/common/");
	fs::create_directory("output/" + theConfiguration.getOutputName() + "/common/countries/");
	fs::create_directory("output/" + theConfiguration.getOutputName() + "/common/country_tags/");
	fs::create_directory("output/" + theConfiguration.getOutputName() + "/localisation/");

	LOG(LogLevel::Info) << "<- Crafting .mod File";
	createModFile(theConfiguration);

	// Record converter version
	LOG(LogLevel::Info) << "<- Writing version";
	outputVersion(versionParser, theConfiguration);

	// Output common\countries.txt
	LOG(LogLevel::Info) << "<- Creating countries.txt";
	outputCommonCountriesFile(theConfiguration);

	LOG(LogLevel::Info) << "<- Writing Countries";
	outputCommonCountries(theConfiguration);
	outputHistoryCountries(theConfiguration);

	LOG(LogLevel::Info) << "<- Writing Provinces";
	outputHistoryProvinces(theConfiguration);

	LOG(LogLevel::Info) << "<- Writing Localization";
	outputLocalization(theConfiguration);

	LOG(LogLevel::Info) << "<- Moving Flags";
	outputFlags(theConfiguration);
}

void EU4::World::outputFlags(const Configuration& theConfiguration) const
{
	for (const auto& country: countries) {
		if (country.second->getTitle().first.empty()) continue; // Probably vanilla nation.
		auto titleName = country.second->getTitle().first;
		std::string fileName;
		if (Utils::DoesFileExist(theConfiguration.getCK2Path() + "/gfx/flags/" + titleName + ".tga"))
			fileName = theConfiguration.getCK2Path() + "/gfx/flags/" + titleName + ".tga";
		if (fileName.empty() && !country.second->getTitle().second->getBaseTitle().first.empty())
		{
			titleName = country.second->getTitle().second->getBaseTitle().first;
			if (Utils::DoesFileExist(theConfiguration.getCK2Path() + "/gfx/flags/" + titleName + ".tga"))
				fileName = theConfiguration.getCK2Path() + "/gfx/flags/" + titleName + ".tga";			
			if (fileName.empty() && !country.second->getTitle().second->getBaseTitle().second->getBaseTitle().first.empty()) {
				titleName = country.second->getTitle().second->getBaseTitle().second->getBaseTitle().first;
				if (Utils::DoesFileExist(theConfiguration.getCK2Path() + "/gfx/flags/" + titleName + ".tga"))
					fileName = theConfiguration.getCK2Path() + "/gfx/flags/" + titleName + ".tga";
			}
		}
		if (fileName.empty()) Log(LogLevel::Warning) << "failed to locate flag for " << country.first << ": " << country.second->getTitle().first;
	}
}

void EU4::World::createModFile(const Configuration& theConfiguration) const
{
	std::ofstream output("output/" + theConfiguration.getOutputName() + ".mod");
	if (!output.is_open()) throw std::runtime_error("Could not create " + theConfiguration.getOutputName() + ".mod");
	LOG(LogLevel::Info) << "<< Writing to: "
							  << "output/" + theConfiguration.getOutputName() + ".mod";
	output << modFile;
	output.close();
}


void EU4::World::outputLocalization(const Configuration& theConfiguration) const
{
	std::ofstream english("output/" + theConfiguration.getOutputName() + "/localisation/converter_l_english.yml");
	std::ofstream french("output/" + theConfiguration.getOutputName() + "/localisation/converter_l_french.yml");
	std::ofstream spanish("output/" + theConfiguration.getOutputName() + "/localisation/converter_l_spanish.yml");
	std::ofstream german("output/" + theConfiguration.getOutputName() + "/localisation/converter_l_german.yml");
	if (!english.is_open()) throw std::runtime_error("Error writing localisation file! Is the output folder writable?");
	if (!french.is_open()) throw std::runtime_error("Error writing localisation file! Is the output folder writable?");
	if (!spanish.is_open()) throw std::runtime_error("Error writing localisation file! Is the output folder writable?");
	if (!german.is_open()) throw std::runtime_error("Error writing localisation file! Is the output folder writable?");
	english << "\xEF\xBB\xBFl_english:\n"; // write BOM
	french << "\xEF\xBB\xBFl_french:\n";	// write BOM
	spanish << "\xEF\xBB\xBFl_spanish:\n"; // write BOM
	german << "\xEF\xBB\xBFl_german:\n";	// write BOM

	for (const auto& country: countries) {
		for (const auto& locblock: country.second->getLocalizations())
		{
			english << " " << locblock.first << ": \"" << Utils::convertWin1252ToUTF8(locblock.second.english) << "\"\n";
			french << " " << locblock.first << ": \"" << Utils::convertWin1252ToUTF8(locblock.second.french) << "\"\n";
			spanish << " " << locblock.first << ": \"" << Utils::convertWin1252ToUTF8(locblock.second.spanish) << "\"\n";
			german << " " << locblock.first << ": \"" << Utils::convertWin1252ToUTF8(locblock.second.german) << "\"\n";
		}
	}
	english.close();
	french.close();
	spanish.close();
	german.close();
}

void EU4::World::outputVersion(const mappers::VersionParser& versionParser, const Configuration& theConfiguration) const
{
	std::ofstream output("output/" + theConfiguration.getOutputName() + "/ck2toeu4_version.txt");
	if (!output.is_open()) throw std::runtime_error("Error writing version file! Is the output folder writable?");
	output << versionParser;
	output.close();
}

void EU4::World::outputCommonCountriesFile(const Configuration& theConfiguration) const
{
	std::ofstream output("output/" + theConfiguration.getOutputName() + "/common/country_tags/00_countries.txt");
	if (!output.is_open()) throw std::runtime_error("Could not create countries file!");
	output << "REB = \"countries/Rebels.txt\"\n\n"; // opening with rebels manually.
	
	for (const auto& country: countries) { if (country.first != "REB") output << country.first << " = \"" << country.second->getCommonCountryFile() << "\"\n"; }
	output << "\n";
	output.close();
}

void EU4::World::outputHistoryProvinces(const Configuration& theConfiguration) const
{
	for (const auto& province: provinces) {
		std::ofstream output("output/" + theConfiguration.getOutputName() + "/" + province.second->getHistoryCountryFile());
		if (!output.is_open())
			throw std::runtime_error("Could not create country history file: output/" + theConfiguration.getOutputName() + "/" + province.second->getHistoryCountryFile());
		output << *province.second;
		output.close();
	}
}

void EU4::World::outputHistoryCountries(const Configuration& theConfiguration) const
{
	for (const auto& country: countries) {
		std::ofstream output("output/" + theConfiguration.getOutputName() + "/" + country.second->getHistoryCountryFile());
		if (!output.is_open())
			throw std::runtime_error("Could not create country history file: output/" + theConfiguration.getOutputName() + "/" + country.second->getHistoryCountryFile());
		output << *country.second;
		output.close();
	}
}

void EU4::World::outputCommonCountries(const Configuration& theConfiguration) const
{
	for (const auto& country: countries) {
		std::ofstream output("output/" + theConfiguration.getOutputName() + "/common/" + country.second->getCommonCountryFile());
		if (!output.is_open())
			throw std::runtime_error("Could not create country common file: output/" + theConfiguration.getOutputName() + "/common/" + country.second->getCommonCountryFile());
		country.second->outputCommons(output);
		output.close();
	}
}
