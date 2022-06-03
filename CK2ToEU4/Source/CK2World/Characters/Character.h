#ifndef CK2_CHARACTER_H
#define CK2_CHARACTER_H
#include "../Provinces/Barony.h"
#include "../Titles/Liege.h"
#include "Date.h"
#include "Parser.h"

namespace CK2
{
class Liege;
class Title;
class Barony;
class Dynasty;
class Province;

typedef struct Skills
{
	int diplomacy = 0;
	int martial = 0;
	int stewardship = 0;
	int intrigue = 0;
	int learning = 0;
} Skills;

class Character: commonItems::parser
{
  public:
	Character(std::istream& theStream, int chrID);

	void setLiege(std::shared_ptr<Character> theLiege) { liege.second = std::move(theLiege); }

	[[nodiscard]] const std::string& getCulture() const;
	[[nodiscard]] const std::string& getReligion() const;
	[[nodiscard]] const auto& getName() const { return name; }
	[[nodiscard]] const auto& getBirthDate() const { return birthDate; }
	[[nodiscard]] const auto& getDeathDate() const { return deathDate; }
	[[nodiscard]] const auto& getSkills() const { return skills; }
	[[nodiscard]] const auto& getSpouses() const { return spouses; }
	[[nodiscard]] const auto& getChildren() const { return children; }
	[[nodiscard]] const auto& getHeir() const { return heir; }
	[[nodiscard]] const auto& getPrimaryTitle() const { return primaryTitle; }
	[[nodiscard]] const auto& getChangedPrimaryTitle() const { return changedPrimaryTitle; }
	[[nodiscard]] const auto& getCapital() const { return capital; }
	[[nodiscard]] const auto& getCapitalProvince() const { return capitalProvince; }
	[[nodiscard]] const auto& getGovernment() const { return government; }
	[[nodiscard]] const auto& getLiege() const { return liege; }
	[[nodiscard]] const auto& getMother() const { return mother; }
	[[nodiscard]] const auto& getFather() const { return father; }
	[[nodiscard]] const auto& getDynasty() const { return dynasty; }
	[[nodiscard]] const auto& getCourtierNames() const { return courtierNames; }
	[[nodiscard]] const auto& getTraits() const { return traits; }
	[[nodiscard]] const auto& getJob() const { return job; }
	[[nodiscard]] const auto& getAdvisers() const { return advisers; }

	[[nodiscard]] auto getPrestige() const { return prestige; }
	[[nodiscard]] auto isFemale() const { return female; }
	[[nodiscard]] auto getPiety() const { return piety; }
	[[nodiscard]] auto getWealth() const { return wealth; }
	[[nodiscard]] auto hasLoan() const { return loan; }
	[[nodiscard]] auto getID() const { return charID; }
	[[nodiscard]] auto getHost() const { return host; }
	[[nodiscard]] auto isSpent() const { return spent; }

	[[nodiscard]] bool hasTrait(const std::string& wantedTrait) const;

	void setDynasty(std::shared_ptr<Dynasty> theDynasty) { dynasty.second = std::move(theDynasty); }
	void setCourtierNames(const std::map<std::string, bool>& theNames) { courtierNames = theNames; }
	void setSpouses(const std::map<int, std::shared_ptr<Character>>& newSpouses) { spouses = newSpouses; }
	void setAdvisers(const std::map<int, std::shared_ptr<Character>>& newAdvisers) { advisers = newAdvisers; }
	void setPrimaryTitle(std::shared_ptr<Title> theTitle) const { primaryTitle.second->setTitle(std::move(theTitle)); }
	void setBaseTitle(std::shared_ptr<Title> theBaseTitle) const { primaryTitle.second->setBaseTitle(std::move(theBaseTitle)); }
	void overridePrimaryTitle(const std::pair<std::string, std::shared_ptr<Title>>& theTitle) { changedPrimaryTitle = theTitle; }
	void setCapitalBarony(std::shared_ptr<Barony> theCapitalBarony) { capital.second = std::move(theCapitalBarony); }
	void insertCapitalProvince(const std::pair<int, std::shared_ptr<Province>>& theProvince) { capitalProvince = theProvince; }
	void setTraits(const std::map<int, std::string>& theTraits) { traits = theTraits; }
	void setMother(const std::pair<int, std::shared_ptr<Character>>& theMother) { mother = theMother; }
	void setHeir(const std::pair<int, std::shared_ptr<Character>>& theHeir) { heir = theHeir; }
	void setFather(const std::pair<int, std::shared_ptr<Character>>& theFather) { father = theFather; }
	void registerChild(const std::pair<int, std::shared_ptr<Character>>& theChild) { children.insert(theChild); }
	void addYears(const int years) { birthDate.subtractYears(years); }
	void setSpent() { spent = true; }

  private:
	void registerKeys();

	int charID = 0;
	int host = 0; // a simple ID of the host Character, no link required.
	bool female = false;
	bool spent = false; // if adviser, is already spent?
	bool loan = false;  // borrowed_from_jews
	double piety = 0;
	double prestige = 0;
	double wealth = 0;
	std::string culture;
	std::string religion;
	std::string name;
	std::string government;
	std::string job;
	Skills skills;
	date birthDate = date("1.1.1");
	date deathDate = date("1.1.1");

	std::pair<int, std::shared_ptr<Dynasty>> dynasty;
	std::pair<int, std::shared_ptr<Character>> liege;
	std::pair<int, std::shared_ptr<Character>> mother;
	std::pair<int, std::shared_ptr<Character>> father;
	std::map<int, std::shared_ptr<Character>> children;
	std::pair<int, std::shared_ptr<Character>> heir;
	std::map<int, std::shared_ptr<Character>> spouses;
	std::pair<std::string, std::shared_ptr<Liege>> primaryTitle;
	std::optional<std::pair<std::string, std::shared_ptr<Title>>> changedPrimaryTitle;
	std::pair<std::string, std::shared_ptr<Barony>> capital;
	std::pair<int, std::shared_ptr<Province>> capitalProvince;
	std::map<std::string, bool> courtierNames; // A simple list of people's names and genders. True=male.
	std::map<int, std::string> traits;
	std::map<int, std::shared_ptr<Character>> advisers;
};
} // namespace CK2

#endif // CK2_CHARACTER_H
