#ifndef CK2_CHARACTER_H
#define CK2_CHARACTER_H
#include "../Provinces/Barony.h"
#include "../Titles/Liege.h"
#include "Date.h"
#include "newParser.h"

namespace CK2
{
class Liege;
class Title;
class Barony;
class Dynasty;
class Province;

typedef struct {
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

	[[nodiscard]] const auto& getCulture() const { return culture; }
	[[nodiscard]] const auto& getReligion() const { return religion; }
	[[nodiscard]] const auto& getName() const { return name; }
	[[nodiscard]] const auto& getBirthDate() const { return birthDate; }
	[[nodiscard]] const auto& getDeathDate() const { return deathDate; }
	[[nodiscard]] const auto& getSkills() const { return skills; }
	[[nodiscard]] const auto& getSpouses() const { return spouses; }
	[[nodiscard]] const auto& getPrimaryTitle() const { return primaryTitle; }
	[[nodiscard]] const auto& getCapital() const { return capital; }
	[[nodiscard]] const auto& getCapitalProvince() const { return capitalProvince; }

	[[nodiscard]] auto getID() const { return charID; }
	[[nodiscard]] const auto& getLiege() const { return liege; }
	[[nodiscard]] const auto& getDynasty() const { return dynasty; }
	void setDynasty(std::shared_ptr<Dynasty> theDynasty) { dynasty.second = std::move(theDynasty); }
	void setSpouses(const std::map<int, std::shared_ptr<Character>>& newSpouses) { spouses = newSpouses; }
	void setPrimaryTitle(std::shared_ptr<Title> theTitle) const { primaryTitle.second->setTitle(std::move(theTitle)); }
	void setBaseTitle(std::shared_ptr<Title> theBaseTitle) const { primaryTitle.second->setBaseTitle(std::move(theBaseTitle)); }
	void setCapitalBarony(std::shared_ptr<Barony> theCapitalBarony) { capital.second = std::move(theCapitalBarony); }
	void insertCapitalProvince(const std::pair<int, std::shared_ptr<Province>>& theProvince) { capitalProvince = theProvince; }

  private:
	void registerKeys();

	int charID = 0;
	std::string culture;
	std::string religion;
	std::string name;
	std::pair<int, std::shared_ptr<Dynasty>> dynasty;
	std::pair<int, std::shared_ptr<Character>> liege;
	Skills skills;
	date birthDate = date("1.1.1");
	date deathDate = date("1.1.1");
	std::map<int, std::shared_ptr<Character>> spouses;
	std::pair<std::string, std::shared_ptr<Liege>> primaryTitle;
	std::pair<std::string, std::shared_ptr<Barony>> capital;
	std::pair<int, std::shared_ptr<Province>> capitalProvince;
};
} // namespace CK2

#endif // CK2_CHARACTER_H
