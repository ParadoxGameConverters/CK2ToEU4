#ifndef CK2_TITLE_H
#define CK2_TITLE_H
#include "newParser.h"
#include <set>
#include "Liege.h"

namespace CK2
{
	class Character;
	class Title: commonItems::parser
	{
	public:
		Title(std::istream& theStream, std::string theName);

		[[nodiscard]] const auto& getName() const { return name; }
		[[nodiscard]] const auto& getLaws() const { return laws; }
		[[nodiscard]] const auto& getLiege() const { return liege; }
		[[nodiscard]] const auto& getDeJureLiege() const { return deJureLiege; }
		[[nodiscard]] const auto& getHolder() const { return holder; }
		[[nodiscard]] auto isInHRE() const { return inHRE; }
		
		void setHolder(std::shared_ptr<Character> theHolder) { holder.second = std::move(theHolder); }
		void setLiegePrimaryTitle(std::shared_ptr<Title> theTitle) const { liege.second->setTitle(std::move(theTitle)); }
		void setLiegeBaseTitle(std::shared_ptr<Title> theBaseTitle) const { liege.second->setBaseTitle(std::move(theBaseTitle)); }
		void setDJLiegePrimaryTitle(std::shared_ptr<Title> theTitle) const { deJureLiege.second->setTitle(std::move(theTitle)); }
		void setDJLiegeBaseTitle(std::shared_ptr<Title> theBaseTitle) const { deJureLiege.second->setBaseTitle(std::move(theBaseTitle)); }
		void setInHRE() { inHRE = true; }
		void overrideLiege() { liege = deJureLiege; }

	private:
		void registerKeys();

		std::pair<int, std::shared_ptr<Character>> holder;
		std::set<std::string> laws;
		std::pair<std::string, std::shared_ptr<Liege>> liege;
		std::pair<std::string, std::shared_ptr<Liege>> deJureLiege;
		std::string name;
		bool inHRE = false;
	};
}

#endif // CK2_TITLE_H
