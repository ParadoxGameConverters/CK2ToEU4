#ifndef CK2_DYNASTIES_H
#define CK2_DYNASTIES_H
#include "newParser.h"
#include "Dynasty.h"

namespace CK2
{
	class Dynasties: commonItems::parser
	{
	public:
		Dynasties() = default;
		Dynasties(std::istream& theStream);
		[[nodiscard]] const auto& getDynasties() const { return dynasties; }
				
	private:
		void registerKeys();

		std::map<int, Dynasty> dynasties;
	};
}

#endif // CK2_DYNASTIES_H
