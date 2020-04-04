#ifndef CK2_TITLES_H
#define CK2_TITLES_H
#include "newParser.h"

namespace CK2
{
	class Title;
	class Characters;
	class Titles: commonItems::parser
	{
	public:
		Titles() = default;
		Titles(std::istream& theStream);
		[[nodiscard]] const auto& getTitles() const { return titles; }

		void linkHolders(const Characters& theCharacters);
		void linkLiegePrimaryTitles();
		void linkVassals();

	private:
		void registerKeys();

		std::map<std::string, std::shared_ptr<Title>> titles;
	};
}

#endif // CK2_TITLES_H
