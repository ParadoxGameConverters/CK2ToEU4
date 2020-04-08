#ifndef EU4_COUNTRY_DETAILS_H
#define EU4_COUNTRY_DETAILS_H

#include "Color.h"
#include "newParser.h"
#include <set>

namespace EU4
{
class CountryDetails: commonItems::parser
{
  public:
	CountryDetails() = default;
	explicit CountryDetails(const std::string& filePath);
	explicit CountryDetails(std::istream& theStream);

	void parseHistory(const std::string& filePath);
	void parseHistory(std::istream& theStream);

	// These values are open to ease management.
	// This is a storage container for EU4::Country.
	// Some of these values are loaded from files, others are adjusted on the fly.
	std::string primaryCulture = "noculture";
	std::set<std::string> acceptedCultures;
	std::string religion = "noreligion";
	int capital = 0;
	bool inHRE = false;
	bool holyRomanEmperor = false;
	bool celestialEmperor = false;
	std::string graphicalCulture = "westerngfx";
	std::string government = "monarchy";
	std::set<std::string> reforms;
	int governmentRank = 1;
	std::string technologyGroup = "western";
	bool fixedCapital = false;
	int mercantilism = 0;
	std::string unitType;
	std::string religiousSchool;
	std::set<std::string> cults;
	double armyProfessionalism = 0;
	std::set<std::string> historicalRivals;
	std::set<std::string> historicalFriends;
	std::string nationalFocus;
	double piety = 0;
	bool elector = false;
	std::string secondaryReligion;
	std::set<std::string> harmonizedReligions;
	commonItems::Color color;
	std::set<std::string> historicalIdeaGroups;
	std::set<std::string> historicalUnits;
	std::string monarchNames; // Unused, we can't load it.
	std::set<std::string> leaderNames;
	std::set<std::string> shipNames;
	std::set<std::string> armyNames;
	std::set<std::string> fleetNames;
	commonItems::Color revolutionaryColor;
	int historicalScore = 0;
	std::string preferredReligion;
	std::string colonialParent;
	int randomChance = 0;
	std::string specialUnitCulture;
	bool all_your_core_are_belong_to_us = false;
	bool rightToBEARArms = false;

  private:
	void registerKeys();
	void registerHistoryKeys();
};
} // namespace EU4

#endif // EU4_COUNTRY_DETAILS_H
