#ifndef CK2_PROVINCES_H
#define CK2_PROVINCES_H
#include "newParser.h"
#include "Province.h"

namespace CK2
{
	class Provinces: commonItems::parser
	{
	public:
		Provinces() = default;
		Provinces(std::istream& theStream);
		[[nodiscard]] const auto& getProvinces() const { return provinces; }
				
	private:
		void registerKeys();

		std::map<int, Province> provinces;
	};
}

#endif // CK2_PROVINCES_H
