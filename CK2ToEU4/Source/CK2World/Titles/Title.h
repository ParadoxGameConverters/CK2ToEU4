#ifndef CK2_TITLE_H
#define CK2_TITLE_H
#include "newParser.h"
#include "Liege.h"
#include <set>

namespace CK2
{
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

		int holder = 0;
		std::set<std::string> laws;
		Liege liege;
		Liege deJureLiege;
		std::string name;
	};
}

#endif // CK2_TITLE_H
