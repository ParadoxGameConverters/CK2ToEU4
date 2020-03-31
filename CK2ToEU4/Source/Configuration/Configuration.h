#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "Date.h"
#include "newParser.h"
#include "../Common/Version.h"
#include "ConfigurationDetails.h"

class Configuration: commonItems::parser
{
public:
	Configuration();
	explicit Configuration(std::istream& theStream);

	void setEndDate(const date& lastDate) { details.endDate = lastDate; }
	void setStartDate(const date& firstDate) { details.startDate = firstDate; }
	void setCK2Version(const Version& theVersion) { details.CK2Version = theVersion; }

	[[nodiscard]] const auto& getSaveGamePath() const { return details.SaveGamePath; }
	[[nodiscard]] const auto& getCK2Path() const { return details.CK2Path; }
	[[nodiscard]] const auto& getCK2ModsPath() const { return details.CK2ModsPath; }
	[[nodiscard]] const auto& getEU4Path() const { return details.EU4Path; }
	[[nodiscard]] const auto& getOutputName() const { return details.outputName; }

private:
	void registerKeys();
	void setOutputName();
	void verifyCK2Path() const;
	void verifyEU4Path() const;

	ConfigurationDetails details;
};

std::string trimPath(const std::string& fileName);
std::string trimExtension(const std::string& fileName);
std::string replaceCharacter(std::string fileName, char character);

#endif // CONFIGURATION_H
