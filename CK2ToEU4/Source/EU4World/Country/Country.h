#ifndef EU4_COUNTRY_H
#define EU4_COUNTRY_H

#include "../../CK2World/Titles/Title.h"
#include "CountryDetails.h"
#include <memory>
#include <string>

namespace EU4
{
class Country
{
  public:
	Country() = default;

	Country(std::string theTag, const std::string& filePath);
	void loadHistory(const std::string& filePath);
	void initializeFromTitle(std::string theTag, std::shared_ptr<CK2::Title> thetitle);
	void outputCommons(std::ostream& output);

	[[nodiscard]] const auto& getCommonCountryFile() const { return commonCountryFile; }
	[[nodiscard]] const auto& getHistoryCountryFile() const { return historyCountryFile; }

	friend std::ostream& operator<<(std::ostream& output, const Country& versionParser);

  private:
	std::string tag;
	std::string commonCountryFile;
	std::string historyCountryFile;
	std::shared_ptr<CK2::Title> title;

	CountryDetails details;
};
} // namespace EU4

#endif // EU4_COUNTRY_H