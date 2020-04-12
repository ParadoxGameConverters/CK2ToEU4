#ifndef EU4_COUNTRY_DETAILS_H
#define EU4_COUNTRY_DETAILS_H

#include "Color.h"
#include "newParser.h"
#include <set>
#include "Date.h"

namespace EU4
{

typedef struct {
	std::string name;
	std::string dynasty;
	int adm = 0;
	int dip = 0;
	int mil = 0;
	date birthDate;
	date deathDate = date("1.1.1");
	bool female = false;
	bool regent = false;
	std::string religion;
	std::string culture;
	bool isSet = false;
	std::string originCountry;
	int claim = 0;
	bool regency = false;
	std::set<std::string> personalities;
} Monarch;
	
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
	std::map<std::string, std::pair<int, int>> monarchNames; // name (without regnal), pair(regnal, chance)
	std::set<std::string> leaderNames;
	std::set<std::string> shipNames;
	std::set<std::string> armyNames;
	std::set<std::string> fleetNames;
	commonItems::Color revolutionaryColor;
	int historicalScore = 0;
	std::string preferredReligion;
	std::string colonialParent;
	bool randomChance = false;
	std::string specialUnitCulture;
	bool all_your_core_are_belong_to_us = false;
	bool rightToBEARArms = false;
	std::vector<std::pair<date, std::string>> historyLessons; // this is used to store history entries for countries we're only transcribing.

	Monarch monarch;
	Monarch queen;
	Monarch heir;
	
  private:
	void registerKeys();
	void registerHistoryKeys();
};
} // namespace EU4

#endif // EU4_COUNTRY_DETAILS_H
