#ifndef COUNTRY_H
#define COUNTRY_H

#include <memory>
#include <string>
#include "CountryDetails.h"

namespace CK2
{
class Title;
}
namespace EU4
{
class Country
{
  public:
	Country() = default;
	virtual ~Country() = default;

	Country(std::string theTag, const std::string& filePath);
	void loadHistory(const std::string& filePath);

  private:
	std::string tag;
	std::string commonCountryFile;
	std::shared_ptr<CK2::Title> title;

	CountryDetails details;
};
} // namespace EU4

#endif // COUNTRY_H