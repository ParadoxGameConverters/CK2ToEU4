#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "Date.h"
#include "newParser.h"
#include "ConfigurationDetails.h"

class Configuration: commonItems::parser
{
public:
	Configuration();
	explicit Configuration(std::istream& theStream);

	[[nodiscard]] const auto& getSaveGamePath() const { return details.SaveGamePath; }
	[[nodiscard]] const auto& getCK2Path() const { return details.CK2Path; }
	[[nodiscard]] const auto& getCK2ModsPath() const { return details.CK2ModsPath; }
	[[nodiscard]] const auto& getEU4Path() const { return details.EU4Path; }
	[[nodiscard]] const auto& getOutputName() const { return details.outputName; }
	[[nodiscard]] const auto& getHRE() const { return details.iAmHre; }
	[[nodiscard]] const auto& getShatterEmpires() const { return details.shatterEmpires; }
	[[nodiscard]] const auto& getShatterLevel() const { return details.shatterLevel; }

private:
	void registerKeys();
	void setOutputName();
	void verifyCK2Path() const;
	void verifyEU4Path() const;

	ConfigurationDetails details;
};

#endif // CONFIGURATION_H
