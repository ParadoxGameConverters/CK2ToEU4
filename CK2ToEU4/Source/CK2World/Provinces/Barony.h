#ifndef CK2_BARONY_H
#define CK2_BARONY_H
#include "newParser.h"
#include <set>

namespace CK2
{
	class Barony: commonItems::parser
	{
	public:		
		Barony(std::istream& theStream, const std::string& baronyName);
		
		[[nodiscard]] auto getBuildingCount() const { return static_cast<int>(buildings.size()); }
		[[nodiscard]] const auto& getName() const { return name; }
		[[nodiscard]] const auto& getType() const { return type; }

	private:
		void registerKeys();

		std::string name;
		std::string type;
		std::set<std::string> buildings;
	};
}

#endif // CK2_BARONY_H
