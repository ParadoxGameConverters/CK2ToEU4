#include "Configuration.h"
#include "CommonFunctions.h"
#include "CommonRegexes.h"
#include "GameVersion.h"
#include "Log.h"
#include "OSCompatibilityLayer.h"
#include "ParserHelpers.h"
#include <fstream>

Configuration::Configuration(const commonItems::ConverterVersion& converterVersion)
{
	Log(LogLevel::Info) << "Reading configuration file";
	registerKeys();
	parseFile("configuration.txt");
	clearRegisteredKeywords();
	setOutputName();
	verifyCK2Path();
	verifyCK2Version(converterVersion);
	verifyEU4Path();
	verifyEU4Version(converterVersion);
	Log(LogLevel::Progress) << "3 %";
}

Configuration::Configuration(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
	setOutputName();
}

void Configuration::registerKeys()
{
	registerKeyword("SaveGame", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString path(theStream);
		SaveGamePath = path.getString();
		Log(LogLevel::Info) << "Save Game set to: " << SaveGamePath.string();
	});
	registerKeyword("CK2directory", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString path(theStream);
		CK2Path = path.getString();
		Log(LogLevel::Info) << "CK2directory set to: " << CK2Path.string();
	});
	registerKeyword("CK2DocDirectory", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString path(theStream);
		CK2DocsPath = path.getString();
		Log(LogLevel::Info) << "CK2DocDirectory set to: " << CK2DocsPath.string();
	});
	registerKeyword("EU4directory", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString path(theStream);
		EU4Path = path.getString();
		Log(LogLevel::Info) << "EU4directory set to: " << EU4Path.string();
	});
	registerKeyword("output_name", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString nameStr(theStream);
		outputName = nameStr.getString();
		Log(LogLevel::Info) << "Output name set to: " << outputName.string();
	});
	registerKeyword("start_date", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString startDateString(theStream);
		startDate = STARTDATE(std::stoi(startDateString.getString()));
		Log(LogLevel::Info) << "Start date set to: " << startDateString.getString();
	});
	registerKeyword("i_am_hre", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString hreString(theStream);
		iAmHre = I_AM_HRE(std::stoi(hreString.getString()));
		Log(LogLevel::Info) << "HRE set to: " << hreString.getString();
	});
	registerKeyword("dejure", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString dejureString(theStream);
		dejure = DEJURE(std::stoi(dejureString.getString()));
		Log(LogLevel::Info) << "DeJure set to: " << dejureString.getString();
	});
	registerKeyword("split_vassals", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString splitVassalsString(theStream);
		splitVassals = SPLITVASSALS(std::stoi(splitVassalsString.getString()));
		Log(LogLevel::Info) << "Split Vassals set to: " << splitVassalsString.getString();
	});
	registerKeyword("shatter_empires", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString shatterEmpiresString(theStream);
		shatterEmpires = SHATTER_EMPIRES(std::stoi(shatterEmpiresString.getString()));
		Log(LogLevel::Info) << "Shatter Empires set to: " << shatterEmpiresString.getString();
	});
	registerKeyword("shatter_level", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString shatterLevelString(theStream);
		shatterLevel = SHATTER_LEVEL(std::stoi(shatterLevelString.getString()));
		Log(LogLevel::Info) << "Shatter Level set to: " << shatterLevelString.getString();
	});
	registerKeyword("shatter_hre_level", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString shatterHRELevelString(theStream);
		shatterHRELevel = SHATTER_HRE_LEVEL(std::stoi(shatterHRELevelString.getString()));
		Log(LogLevel::Info) << "Shatter HRE Level set to: " << shatterHRELevelString.getString();
	});
	registerKeyword("siberia", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString siberiaString(theStream);
		siberia = SIBERIA(std::stoi(siberiaString.getString()));
		Log(LogLevel::Info) << "Siberia set to: " << siberiaString.getString();
	});
	registerKeyword("sunset", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString sunsetString(theStream);
		sunset = SUNSET(std::stoi(sunsetString.getString()));
		Log(LogLevel::Info) << "Sunset set to: " << sunsetString.getString();
	});
	registerKeyword("dynamicInstitutions", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString dynamicInstitutionsString(theStream);
		dynamicInstitutions = INSTITUTIONS(std::stoi(dynamicInstitutionsString.getString()));
		Log(LogLevel::Info) << "Dynamic Institutions set to: " << dynamicInstitutionsString.getString();
	});
	registerKeyword("development", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString developmentString(theStream);
		development = DEVELOPMENT(std::stoi(developmentString.getString()));
		Log(LogLevel::Info) << "Development set to: " << developmentString.getString();
	});
	registerKeyword("selectedMods", [this](const std::string& unused, std::istream& theStream) {
		for (const auto& path: commonItems::getStrings(theStream))
			mods.emplace_back(Mod("", path));
		Log(LogLevel::Info) << mods.size() << " mods selected by configuration. Deselected mods will be ignored.";
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}

void Configuration::verifyCK2Path() const
{
	if (!commonItems::DoesFolderExist(CK2Path))
		throw std::runtime_error(CK2Path.string() + " does not exist!");
	if (!commonItems::DoesFileExist(CK2Path / "CK2game.exe") && !commonItems::DoesFileExist(CK2Path / "CK2game") && !commonItems::DoesFileExist(CK2Path / "ck2"))
		throw std::runtime_error(CK2Path.string() + " does not contain Crusader Kings 2!");
	if (!commonItems::DoesFileExist(CK2Path / "map/positions.txt"))
		throw std::runtime_error(CK2Path.string() + " does not appear to be a valid CK2 install!");
	Log(LogLevel::Info) << "\tCK2 install path is " << CK2Path.string();
}

void Configuration::verifyEU4Path() const
{
	if (!commonItems::DoesFolderExist(EU4Path))
		throw std::runtime_error(EU4Path.string() + " does not exist!");
	if (!commonItems::DoesFileExist(EU4Path / "eu4.exe") && !commonItems::DoesFileExist(EU4Path / "eu4"))
		throw std::runtime_error(EU4Path.string() + " does not contain Europa Universalis 4!");
	if (!commonItems::DoesFileExist(EU4Path / "map/positions.txt"))
		throw std::runtime_error(EU4Path.string() + " does not appear to be a valid EU4 install!");
	Log(LogLevel::Info) << "\tEU4 install path is " << EU4Path.string();
}

void Configuration::setOutputName()
{
	std::string outputNameString;
	if (outputName.empty())
	{
		outputNameString = SaveGamePath.filename().stem().string();
	}
	else
	{
		outputNameString = outputName.string();
	}
	outputNameString = replaceCharacter(outputNameString, '-');
	outputNameString = replaceCharacter(outputNameString, ' ');

	outputName = commonItems::normalizeUTF8Path(outputNameString);
	Log(LogLevel::Info) << "Using output name " << outputName.string();
}

void Configuration::verifyCK2Version(const commonItems::ConverterVersion& converterVersion) const
{
	const auto CK2Version = GameVersion::extractVersionFromChangeLog(CK2Path / "ChangeLog.txt");
	if (!CK2Version)
	{
		Log(LogLevel::Error) << "CK2 version could not be determined, proceeding blind!";
		return;
	}

	Log(LogLevel::Info) << "CK2 version: " << CK2Version->toShortString();

	if (converterVersion.getMinSource() > *CK2Version)
	{
		Log(LogLevel::Error) << "CK2 version is v" << CK2Version->toShortString() << ", converter requires minimum v"
									<< converterVersion.getMinSource().toShortString() << "!";
		throw std::runtime_error("Converter vs CK2 installation mismatch!");
	}
	if (!converterVersion.getMaxSource().isLargerishThan(*CK2Version))
	{
		Log(LogLevel::Error) << "CK2 version is v" << CK2Version->toShortString() << ", converter requires maximum v"
									<< converterVersion.getMaxSource().toShortString() << "!";
		throw std::runtime_error("Converter vs CK2 installation mismatch!");
	}
}

void Configuration::verifyEU4Version(const commonItems::ConverterVersion& converterVersion) const
{
	const auto EU4Version = GameVersion::extractVersionFromLauncher(EU4Path / "launcher-settings.json");
	if (!EU4Version)
	{
		Log(LogLevel::Error) << "EU4 version could not be determined, proceeding blind!";
		return;
	}

	Log(LogLevel::Info) << "EU4 version: " << EU4Version->toShortString();

	if (converterVersion.getMinTarget() > *EU4Version)
	{
		Log(LogLevel::Error) << "EU4 version is v" << EU4Version->toShortString() << ", converter requires minimum v"
									<< converterVersion.getMinTarget().toShortString() << "!";
		throw std::runtime_error("Converter vs EU4 installation mismatch!");
	}
	if (!converterVersion.getMaxTarget().isLargerishThan(*EU4Version))
	{
		Log(LogLevel::Error) << "EU4 version is v" << EU4Version->toShortString() << ", converter requires maximum v"
									<< converterVersion.getMaxTarget().toShortString() << "!";
		throw std::runtime_error("Converter vs EU4 installation mismatch!");
	}
}
