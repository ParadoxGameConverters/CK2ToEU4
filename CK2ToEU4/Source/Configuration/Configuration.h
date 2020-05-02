#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "Date.h"
#include "newParser.h"

class Configuration: commonItems::parser
{
  public:
	Configuration();
	explicit Configuration(std::istream& theStream);

	enum class DEVELOPMENT
	{
		IMPORT = 1,
		VANILLA = 2
	};
	enum class SUNSET
	{
		DEFAULT = 1,
		DISABLED = 2,
		ACTIVE = 3
	};
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
		ALL = 2,
		CUSTOM = 3
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

	[[nodiscard]] const auto& getSaveGamePath() const { return SaveGamePath; }
	[[nodiscard]] const auto& getCK2Path() const { return CK2Path; }
	[[nodiscard]] const auto& getCK2ModsPath() const { return CK2ModsPath; }
	[[nodiscard]] const auto& getEU4Path() const { return EU4Path; }
	[[nodiscard]] const auto& getOutputName() const { return outputName; }
	[[nodiscard]] const auto& getHRE() const { return iAmHre; }
	[[nodiscard]] const auto& getShatterEmpires() const { return shatterEmpires; }
	[[nodiscard]] const auto& getShatterLevel() const { return shatterLevel; }
	[[nodiscard]] const auto& getShatterHRELevel() const { return shatterHRELevel; }
	[[nodiscard]] const auto& getSiberia() const { return siberia; }
	[[nodiscard]] const auto& getSunset() const { return sunset; }
	[[nodiscard]] const auto& getDevelopment() const { return development; }

  private:
	void registerKeys();
	void setOutputName();
	void verifyCK2Path() const;
	void verifyEU4Path() const;

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
	SUNSET sunset = SUNSET::DEFAULT;
	DEVELOPMENT development = DEVELOPMENT::IMPORT;
};

#endif // CONFIGURATION_H
