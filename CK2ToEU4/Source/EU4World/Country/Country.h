#ifndef COUNTRY_H
#define COUNTRY_H

#include <memory>
#include <string>
#include "CountryDetails.h"
#include "../../CK2World/Titles/Title.h"

namespace EU4
{
class Country
{
  public:
	Country() = default;

	Country(std::string theTag, const std::string& filePath);
	void loadHistory(const std::string& filePath);
	void initializeFromTitle(std::string theTag, std::shared_ptr<CK2::Title> thetitle);
	
  private:
	std::string tag;
	std::string commonCountryFile;
	std::shared_ptr<CK2::Title> title;

	CountryDetails details;
};
} // namespace EU4

#endif // COUNTRY_H