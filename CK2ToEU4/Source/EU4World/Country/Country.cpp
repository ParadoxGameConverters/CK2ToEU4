#include "Country.h"
#include "../../CK2World/Characters/Character.h"
#include "../../CK2World/Titles/Title.h"
#include "../../Mappers/GovernmentsMapper/GovernmentsMapper.h"
#include "Log.h"

EU4::Country::Country(std::string theTag, const std::string& filePath): tag(std::move(theTag))
{
	// Load from a country file, if one exists. Otherwise rely on defaults.
	const auto startPos = filePath.find("/countries");
	commonCountryFile = filePath.substr(startPos + 1, filePath.length() - startPos);
	details = CountryDetails(filePath);
}

void EU4::Country::loadHistory(const std::string& filePath)
{
	const auto startPos = filePath.find("/history");
	historyCountryFile = filePath.substr(startPos + 1, filePath.length() - startPos);
	details.parseHistory(filePath);
}

void EU4::Country::initializeFromTitle(std::string theTag, std::shared_ptr<CK2::Title> thetitle, const mappers::GovernmentsMapper& governmentsMapper)
{
	tag = std::move(theTag);
	title = std::move(thetitle);
	if (commonCountryFile.empty()) commonCountryFile = "countries/" + title->getName() + ".txt";
	if (historyCountryFile.empty()) historyCountryFile = "history/countries/" + tag + " - " + title->getName() + ".txt";

	// History section
	details.government.clear();
	details.reforms.clear();
	const auto& newGovernment = governmentsMapper.matchGovernment(title->getHolder().second->getGovernment(), title->getName());
	if (newGovernment) {
		details.government = newGovernment->first;
		if (!newGovernment->second.empty()) details.reforms.insert(newGovernment->second);
		if (!newGovernment->second.empty()) Log(LogLevel::Debug) << tag << " setting government reform: " << newGovernment->second;
	} else {
		Log(LogLevel::Warning) << "No government match for " << title->getHolder().second->getGovernment() << " for title: " << title->getName() << ", defaulting to monarchy.";
		details.government = "monarchy";
	}


	// Common section
}
