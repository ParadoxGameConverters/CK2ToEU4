#include "Configuration.h"
#include "../Common/CommonFunctions.h"
#include "Log.h"
#include "OSCompatibilityLayer.h"
#include "ParserHelpers.h"

Configuration::Configuration()
{
	LOG(LogLevel::Info) << "Reading configuration file";
	registerKeys();
	parseFile("configuration.txt");
	clearRegisteredKeywords();
	setOutputName();
	verifyCK2Path();
	verifyEU4Path();
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
	registerKeyword("configuration", [this](const std::string& unused, std::istream& theStream) {
		details = ConfigurationDetails(theStream);
	});
	registerRegex("[a-zA-Z0-9\\_.:]+", commonItems::ignoreItem);
}


void Configuration::verifyCK2Path() const
{
	if (!Utils::doesFolderExist(details.CK2Path))
		throw std::runtime_error(details.CK2Path + " does not exist!");
	if (!Utils::DoesFileExist(details.CK2Path + "/CK2game.exe") && !Utils::DoesFileExist(details.CK2Path + "/CK2game"))
		throw std::runtime_error(details.CK2Path + " does not contain Crusader Kings 2!");
	if (!Utils::DoesFileExist(details.CK2Path + "/map/positions.txt"))
		throw std::runtime_error(details.CK2Path + " does not appear to be a valid CK2 install!");
	LOG(LogLevel::Info) << "\tCK2 install path is " << details.CK2Path;
}

void Configuration::verifyEU4Path() const
{
	if (!Utils::doesFolderExist(details.EU4Path))
		throw std::runtime_error(details.EU4Path + " does not exist!");
	if (!Utils::DoesFileExist(details.EU4Path + "/eu4.exe") && !Utils::DoesFileExist(details.EU4Path + "/eu4"))
		throw std::runtime_error(details.EU4Path + " does not contain Europa Universalis 4!");
	if (!Utils::DoesFileExist(details.EU4Path + "/map/positions.txt"))
		throw std::runtime_error(details.EU4Path + " does not appear to be a valid EU4 install!");
	LOG(LogLevel::Info) << "\tEU4 install path is " << details.EU4Path;
}

void Configuration::setOutputName()
{
	if (details.outputName.empty())
	{
		details.outputName = trimPath(details.SaveGamePath);
	}
	details.outputName = trimExtension(details.outputName);
	details.outputName = replaceCharacter(details.outputName, '-');
	details.outputName = replaceCharacter(details.outputName, ' ');

	details.outputName = Utils::normalizeUTF8Path(details.outputName);
	LOG(LogLevel::Info) << "Using output name " << details.outputName;
}
