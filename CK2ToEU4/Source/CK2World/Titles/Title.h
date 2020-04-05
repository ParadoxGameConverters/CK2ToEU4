#ifndef CK2_TITLE_H
#define CK2_TITLE_H
#include "newParser.h"
#include <set>
#include "Liege.h"

namespace CK2
{
	class Character;
	class Province;
	class Title: commonItems::parser
	{
	public:
		Title(std::istream& theStream, std::string theName);

		[[nodiscard]] const auto& getName() const { return name; }
		[[nodiscard]] const auto& getLaws() const { return laws; }
		[[nodiscard]] const auto& getLiege() const { return liege; }
		[[nodiscard]] const auto& getDeJureLiege() const { return deJureLiege; }
		[[nodiscard]] const auto& getHolder() const { return holder; }
		[[nodiscard]] const auto& getVassals() const { return vassals; }
		[[nodiscard]] const auto& getDeJureVassals() const { return deJureVassals; }
		[[nodiscard]] const auto& getProvinces() const { return provinces; }
		[[nodiscard]] const auto& getBaseTitle() const { return baseTitle; }
		[[nodiscard]] auto isInHRE() const { return inHRE; }
		[[nodiscard]] auto isMajorRevolt() const { return majorRevolt; }

		[[nodiscard]] std::map<int, std::shared_ptr<Province>> coalesceProvinces() const;
		
		void setHolder(std::shared_ptr<Character> theHolder) { holder.second = std::move(theHolder); }
		void setLiegePrimaryTitle(std::shared_ptr<Title> theTitle) const { liege.second->setTitle(std::move(theTitle)); }
		void setBaseTitleTitle(std::shared_ptr<Title> theTitle) const { baseTitle.second->setTitle(std::move(theTitle)); }
		void setBaseTitleBaseTitle(std::shared_ptr<Title> theTitle) const { baseTitle.second->setBaseTitle(std::move(theTitle)); }
		void setLiegeBaseTitle(std::shared_ptr<Title> theBaseTitle) const { liege.second->setBaseTitle(std::move(theBaseTitle)); }
		void setDJLiegePrimaryTitle(std::shared_ptr<Title> theTitle) const { deJureLiege.second->setTitle(std::move(theTitle)); }
		void setDJLiegeBaseTitle(std::shared_ptr<Title> theBaseTitle) const { deJureLiege.second->setBaseTitle(std::move(theBaseTitle)); }
		void setInHRE() { inHRE = true; }
		void overrideLiege() { liege = deJureLiege; }
		void overrideLiege(const std::pair<std::string, std::shared_ptr<Liege>>& theLiege) { liege = theLiege; }
		void registerVassal(const std::pair<std::string, std::shared_ptr<Title>>& theVassal) { vassals.insert(theVassal); }
		void registerDeJureVassal(const std::pair<std::string, std::shared_ptr<Title>>& theVassal) { deJureVassals.insert(theVassal); }
		void registerProvince(const std::pair<int, std::shared_ptr<Province>>& theProvince) { provinces.insert(theProvince); }
		void congregateProvinces(const std::map<std::string, std::shared_ptr<Title>>& independentTitles);
		
	private:
		void registerKeys();

		std::pair<int, std::shared_ptr<Character>> holder;
		std::set<std::string> laws;
		std::pair<std::string, std::shared_ptr<Liege>> liege;
		std::pair<std::string, std::shared_ptr<Liege>> deJureLiege;
		std::string name;
		bool inHRE = false;
		bool majorRevolt = false;
		std::map<int, std::shared_ptr<Province>> provinces;
		std::map<std::string, std::shared_ptr<Title>> vassals;
		std::map<std::string, std::shared_ptr<Title>> deJureVassals;
		std::pair<std::string, std::shared_ptr<Liege>> baseTitle;
	};
}

#endif // CK2_TITLE_H
