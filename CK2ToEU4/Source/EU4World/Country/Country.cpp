#include "Country.h"
#include "Log.h"

EU4::Country::Country(std::string theTag, const std::string& filePath): tag(std::move(theTag))
{
	// Load from a country file, if one exists. Otherwise rely on defaults.
	const auto startPos = filePath.find("/countries");
	commonCountryFile = filePath.substr(startPos, filePath.length() - startPos);
	details = CountryDetails(filePath);
}

void EU4::Country::loadHistory(const std::string& filePath)
{
	details.parseHistory(filePath);
}

void EU4::Country::initializeFromTitle(std::string theTag, std::shared_ptr<CK2::Title> thetitle)
{
	tag = std::move(theTag);
	title = std::move(thetitle);
	if (commonCountryFile.empty()) commonCountryFile = "/countries/" + title->getName() + ".txt";

	// History section

	// Common section
}
