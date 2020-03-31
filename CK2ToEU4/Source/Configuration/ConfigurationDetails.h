#ifndef CONFIGURATION_DETAILS_H
#define CONFIGURATION_DETAILS_H

#include "Date.h"
#include "newParser.h"
#include "../Common/Version.h"

class ConfigurationDetails: commonItems::parser
{
public:
	ConfigurationDetails() = default;
	explicit ConfigurationDetails(std::istream& theStream);

	std::string SaveGamePath;
	std::string CK2Path;
	std::string CK2ModsPath;
	std::string EU4Path;
	std::string outputName;

	date endDate = date("1444.11.11");
	date startDate = date("1.1.1");
	Version CK2Version;

private:
	void registerKeys();
	
};

#endif // CONFIGURATION_DETAILS_H
