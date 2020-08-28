#ifndef CK2_TITLE_H
#define CK2_TITLE_H
#include "Color.h"
#include "Liege.h"
#include "Parser.h"
#include <set>

namespace EU4
{
class Country;
} // namespace EU4

namespace CK2
{
class Character;
class Province;
class Title: commonItems::parser
{
  public:
	Title(std::istream& theStream, std::string theName);

	[[nodiscard]] const auto& getName() const { return name; }
	[[nodiscard]] const auto& getDisplayName() const { return displayName; }
	[[nodiscard]] const auto& getLaws() const { return laws; }
	[[nodiscard]] const auto& getLiege() const { return liege; }
	[[nodiscard]] const auto& getDeJureLiege() const { return deJureLiege; }
	[[nodiscard]] const auto& getHolder() const { return holder; }
	[[nodiscard]] const auto& getVassals() const { return vassals; }
	[[nodiscard]] const auto& getGeneratedVassals() const { return generatedVassals; }
	[[nodiscard]] const auto& getGeneratedLiege() const { return generatedLiege; }
	[[nodiscard]] const auto& getDeJureVassals() const { return deJureVassals; }
	[[nodiscard]] const auto& getProvinces() const { return provinces; }
	[[nodiscard]] const auto& getDeJureProvinces() const { return deJureProvinces; }
	[[nodiscard]] const auto& getBaseTitle() const { return baseTitle; }
	[[nodiscard]] const auto& getColor() const { return color; }
	[[nodiscard]] const auto& getGenderLaw() const { return genderLaw; }
	[[nodiscard]] const auto& getSuccessionLaw() const { return successionLaw; }
	[[nodiscard]] const auto& getEU4Tag() const { return tagCountry; }
	[[nodiscard]] const auto& getPreviousHolders() const { return previousHolders; }
	[[nodiscard]] const auto& getElectors() const { return electors; }

	[[nodiscard]] auto isInHRE() const { return inHRE; }
	[[nodiscard]] auto isHREEmperor() const { return HREEmperor; }
	[[nodiscard]] auto isThePope() const { return thePope; }
	[[nodiscard]] auto isTheFraticelliPope() const { return theFraticelliPope; }
	[[nodiscard]] auto isElector() const { return electorate; }
	[[nodiscard]] auto isMajorRevolt() const { return majorRevolt; }

	[[nodiscard]] std::map<int, std::shared_ptr<Province>> coalesceProvinces() const;
	[[nodiscard]] std::map<int, std::shared_ptr<Province>> coalesceDeJureProvinces() const;
	[[nodiscard]] int flagDeJureHREProvinces();

	void congregateProvinces(const std::map<std::string, std::shared_ptr<Title>>& independentTitles);
	void congregateDeJureProvinces();
	void setHolder(std::shared_ptr<Character> theHolder) { holder.second = std::move(theHolder); }
	void setLiegePrimaryTitle(std::shared_ptr<Title> theTitle) const { liege.second->setTitle(std::move(theTitle)); }
	void setBaseTitleTitle(std::shared_ptr<Title> theTitle) const { baseTitle.second->setTitle(std::move(theTitle)); }
	void setBaseTitleBaseTitle(std::shared_ptr<Title> theTitle) const { baseTitle.second->setBaseTitle(std::move(theTitle)); }
	void setLiegeBaseTitle(std::shared_ptr<Title> theBaseTitle) const { liege.second->setBaseTitle(std::move(theBaseTitle)); }
	void setDJLiegePrimaryTitle(std::shared_ptr<Title> theTitle) const { deJureLiege.second->setTitle(std::move(theTitle)); }
	void setDJLiegeBaseTitle(std::shared_ptr<Title> theBaseTitle) const { deJureLiege.second->setBaseTitle(std::move(theBaseTitle)); }
	void setInHRE() { inHRE = true; }
	void setHREEmperor() { HREEmperor = true; }
	void setThePope() { thePope = true; }
	void setTheFraticelliPope() { theFraticelliPope = true; }
	void setElectorate() { electorate = true; }
	void setPreviousHolders(const std::map<int, std::shared_ptr<Character>>& thePreviousHolders) { previousHolders = thePreviousHolders; }
	void overrideLiege() { liege = deJureLiege; }
	void overrideLiege(const std::pair<std::string, std::shared_ptr<Liege>>& theLiege) { liege = theLiege; }
	void registerGeneratedLiege(const std::pair<std::string, std::shared_ptr<Title>>& liege) { generatedLiege = liege; }
	void registerVassal(const std::pair<std::string, std::shared_ptr<Title>>& theVassal) { vassals.insert(theVassal); }
	void registerGeneratedVassal(const std::pair<std::string, std::shared_ptr<Title>>& theVassal);
	void registerDeJureVassal(const std::pair<std::string, std::shared_ptr<Title>>& theVassal) { deJureVassals.insert(theVassal); }
	void registerProvince(const std::pair<int, std::shared_ptr<Province>>& theProvince) { provinces.insert(theProvince); }
	void registerDeJureProvince(const std::pair<int, std::shared_ptr<Province>>& theProvince) { deJureProvinces.insert(theProvince); }
	void registerEU4Tag(const std::pair<std::string, std::shared_ptr<EU4::Country>>& theCountry) { tagCountry = theCountry; }
	void clearVassals() { vassals.clear(); }
	void clearGeneratedVassals() { generatedVassals.clear(); }
	void clearLiege()
	{
		liege.first.clear();
		liege.second = nullptr;
	}
	void clearHolder()
	{
		holder.first = 0;
		holder.second = nullptr;
	}

  private:
	void registerKeys();

	bool inHRE = false;
	bool HREEmperor = false;
	bool thePope = false;
	bool theFraticelliPope = false;
	bool majorRevolt = false;
	bool electorate = false;
	std::string name;			 // nominal name, k_something
	std::string displayName; // visual name, "Cumania"
	std::string genderLaw;	 // for succession
	std::string successionLaw;
	std::optional<commonItems::Color> color;

	std::set<std::string> laws;
	std::set<int> electors;
	std::map<int, std::shared_ptr<Province>> provinces;
	std::map<int, std::shared_ptr<Province>> deJureProvinces;
	std::map<std::string, std::shared_ptr<Title>> vassals;
	std::map<std::string, std::shared_ptr<Title>> deJureVassals;
	std::map<int, std::shared_ptr<Character>> previousHolders;
	std::map<std::string, std::shared_ptr<Title>> generatedVassals; // Vassals we split off deliberately.
	std::pair<int, std::shared_ptr<Character>> holder;
	std::pair<std::string, std::shared_ptr<Liege>> liege;
	std::pair<std::string, std::shared_ptr<Liege>> deJureLiege;
	std::pair<std::string, std::shared_ptr<Liege>> baseTitle;
	std::pair<std::string, std::shared_ptr<EU4::Country>> tagCountry;
	std::pair<std::string, std::shared_ptr<Title>> generatedLiege; // Liege we set manually.
};
} // namespace CK2

#endif // CK2_TITLE_H
