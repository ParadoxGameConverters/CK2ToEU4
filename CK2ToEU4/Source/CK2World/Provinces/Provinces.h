#ifndef CK2_PROVINCES_H
#define CK2_PROVINCES_H
#include "newParser.h"

namespace CK2
{
	class Province;
	class Provinces: commonItems::parser
	{
	public:
		Provinces() = default;
		Provinces(std::istream& theStream);
		[[nodiscard]] const auto& getProvinces() const { return provinces; }

		void linkPrimarySettlements();

	private:
		void registerKeys();

		std::map<int, std::shared_ptr<Province>> provinces;
	};
}

#endif // CK2_PROVINCES_H
