#include "ConfigurationDetails.h"
#include "ParserHelpers.h"
#include "Log.h"

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
	registerKeyword("CK2directory", [this](const std::string& unused, std::istream& theStream){
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
	registerRegex("[a-zA-Z0-9\\_.:]+", commonItems::ignoreItem);
}
