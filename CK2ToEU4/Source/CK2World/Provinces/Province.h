#ifndef CK2_PROVINCE_H
#define CK2_PROVINCE_H
#include "newParser.h"

class Configuration;

namespace CK2
{
	class Province: commonItems::parser
	{
	public:
		Province(std::istream& theStream, int provID);

		[[nodiscard]] const auto& getCulture() const { return culture; }
		[[nodiscard]] const auto& getReligion() const { return religion; }
		[[nodiscard]] const auto& getName() const { return name; }
		[[nodiscard]] const auto& getPrimarySettlement() const { return primarySettlement; }
		[[nodiscard]] auto getID() const { return provinceID; }

	private:
		void registerKeys();

		int provinceID = 0;
		std::string culture;
		std::string religion;
		std::string name;
		std::string primarySettlement;
		
	};
}

#endif // CK2_PROVINCE_H
