#ifndef CK2_CHARACTER_H
#define CK2_CHARACTER_H
#include "newParser.h"
#include "Date.h"

namespace CK2
{
	class Character: commonItems::parser
	{
	public:
		Character(std::istream& theStream, int chrID);

		[[nodiscard]] const auto& getCulture() const { return culture; }
		[[nodiscard]] const auto& getReligion() const { return religion; }
		[[nodiscard]] const auto& getName() const { return name; }
		[[nodiscard]] const auto& getBirthDate() const { return birthDate; }
		[[nodiscard]] const auto& getDeathDate() const { return deathDate; }

		[[nodiscard]] auto getID() const { return charID; }
		[[nodiscard]] auto getDynasty() const { return dynasty; }

	private:
		void registerKeys();

		int charID = 0;
		std::string culture;
		std::string religion;
		std::string name;
		int dynasty = 0;
		date birthDate = date("1.1.1");
		date deathDate = date("1.1.1");
	};
}

#endif // CK2_CHARACTER_H
