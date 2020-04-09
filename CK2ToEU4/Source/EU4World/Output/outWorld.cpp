#include "../EU4World.h"
#include "Log.h"
#include <filesystem>
#include <fstream>
namespace fs = std::filesystem;
#include "../../Configuration/Configuration.h"

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

	for (const auto& country: countries) { output << country.first << " = \"" << country.second->getCommonCountryFile() << "\"\n"; }
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
