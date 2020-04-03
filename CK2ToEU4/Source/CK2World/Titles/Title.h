#ifndef CK2_TITLE_H
#define CK2_TITLE_H
#include "newParser.h"
#include <set>

namespace CK2
{
	class Liege;
	class Character;
	class Title: commonItems::parser
	{
	public:
		Title(std::istream& theStream, std::string theName);

		[[nodiscard]] const auto& getName() const { return name; }
		[[nodiscard]] const auto& getLaws() const { return laws; }
		[[nodiscard]] const auto& getLiege() const { return liege; }
		[[nodiscard]] const auto& getDeJureLiege() const { return deJureLiege; }

		[[nodiscard]] auto getHolder() const { return holder; }

	private:
		void registerKeys();

		std::pair<int, std::shared_ptr<Character>> holder;
		std::set<std::string> laws;
		std::pair<std::string, std::shared_ptr<Liege>> liege;
		std::pair<std::string, std::shared_ptr<Liege>> deJureLiege;
		std::string name;
	};
}

#endif // CK2_TITLE_H
