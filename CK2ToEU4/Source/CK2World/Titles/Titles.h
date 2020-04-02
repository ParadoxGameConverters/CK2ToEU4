#ifndef CK2_TITLES_H
#define CK2_TITLES_H
#include "newParser.h"
#include "Title.h"

namespace CK2
{
	class Titles: commonItems::parser
	{
	public:
		Titles() = default;
		Titles(std::istream& theStream);
		[[nodiscard]] const auto& getTitles() const { return titles; }
				
	private:
		void registerKeys();

		std::map<std::string, Title> titles;
	};
}

#endif // CK2_TITLES_H
