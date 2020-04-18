#include "ConfigurationDetails.h"
#include "Log.h"
#include "ParserHelpers.h"

ConfigurationDetails::ConfigurationDetails(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void ConfigurationDetails::registerKeys()
{
	registerKeyword("SaveGame", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString path(theStream);
		SaveGamePath = path.getString();
		Log(LogLevel::Info) << "Save Game set to: " << SaveGamePath;
	});
	registerKeyword("CK2directory", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString path(theStream);
		CK2Path = path.getString();
	});
	registerKeyword("CK2ModsDirectory", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString path(theStream);
		CK2ModsPath = path.getString();
	});
	registerKeyword("EU4directory", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString path(theStream);
		EU4Path = path.getString();
	});
	registerKeyword("output_name", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString nameStr(theStream);
		outputName = nameStr.getString();
		Log(LogLevel::Info) << "Output name set to: " << outputName;
	});
	registerKeyword("i_am_hre", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleInt hreInt(theStream);
		iAmHre = I_AM_HRE(hreInt.getInt());
		Log(LogLevel::Info) << "HRE set to: " << hreInt.getInt();
	});
	registerKeyword("shatter_empires", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleInt shatterEmpiresInt(theStream);
		shatterEmpires = SHATTER_EMPIRES(shatterEmpiresInt.getInt());
		Log(LogLevel::Info) << "Shatter Empires set to: " << shatterEmpiresInt.getInt();
	});
	registerKeyword("shatter_level", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleInt shatterLevelInt(theStream);
		shatterLevel = SHATTER_LEVEL(shatterLevelInt.getInt());
		Log(LogLevel::Info) << "Shatter Level set to: " << shatterLevelInt.getInt();
	});
	registerKeyword("siberia", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleInt siberiaInt(theStream);
		siberia = SIBERIA(siberiaInt.getInt());
		Log(LogLevel::Info) << "Siberia set to: " << siberiaInt.getInt();
	});
	registerRegex("[a-zA-Z0-9\\_.:]+", commonItems::ignoreItem);
}
