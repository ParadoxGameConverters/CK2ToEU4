#ifndef CONFIGURATION_DETAILS_H
#define CONFIGURATION_DETAILS_H

#include "Date.h"
#include "newParser.h"

class ConfigurationDetails: commonItems::parser
{
  public:
	ConfigurationDetails() = default;
	explicit ConfigurationDetails(std::istream& theStream);

	enum class I_AM_HRE
	{
		HRE = 1,
		BYZANTIUM = 2,
		ROME = 3,
		CUSTOM = 4,
		NONE = 5
	};
	enum class SHATTER_EMPIRES
	{
		NONE = 1,
		ALL = 2
	};
	enum class SHATTER_LEVEL
	{
		DUTCHY = 1,
		KINGDOM = 2
	};
	enum class SHATTER_HRE_LEVEL
	{
		DUTCHY = 1,
		KINGDOM = 2
	};
	enum class SIBERIA
	{
		CLEAR_SIBERIA = 1,
		LEAVE_SIBERIA = 2
	};

	std::string SaveGamePath;
	std::string CK2Path;
	std::string CK2ModsPath;
	std::string EU4Path;
	std::string outputName;

	I_AM_HRE iAmHre = I_AM_HRE::HRE;
	SHATTER_EMPIRES shatterEmpires = SHATTER_EMPIRES::NONE;
	SHATTER_HRE_LEVEL shatterHRELevel = SHATTER_HRE_LEVEL::DUTCHY;
	SHATTER_LEVEL shatterLevel = SHATTER_LEVEL::DUTCHY;
	SIBERIA siberia = SIBERIA::CLEAR_SIBERIA;

  private:
	void registerKeys();
};

#endif // CONFIGURATION_DETAILS_H
