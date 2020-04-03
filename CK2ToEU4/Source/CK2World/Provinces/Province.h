#ifndef CK2_PROVINCE_H
#define CK2_PROVINCE_H
#include "newParser.h"

namespace CK2
{
	class Barony;
	class Title;
	class Province: commonItems::parser
	{
	public:
		Province(std::istream& theStream, int provID);

		[[nodiscard]] const auto& getCulture() const { return culture; }
		[[nodiscard]] const auto& getReligion() const { return religion; }
		[[nodiscard]] const auto& getName() const { return name; }
		[[nodiscard]] const auto& getPrimarySettlement() const { return primarySettlement; }
		[[nodiscard]] const auto& getBaronies() const { return baronies; }

		[[nodiscard]] auto getID() const { return provinceID; }
		[[nodiscard]] auto getMaxSettlements() const { return maxSettlements; }
		[[nodiscard]] auto getBaronyCount() const { return static_cast<int>(baronies.size()); }
		[[nodiscard]] int getBuildingWeight() const;

	private:
		void registerKeys();

		int provinceID = 0;
		std::string culture;
		std::string religion;
		std::string name;
		std::pair<std::string, std::shared_ptr<Title>> primarySettlement;
		int maxSettlements = 0;
		std::map<std::string, std::shared_ptr<Barony>> baronies;
	};
}

#endif // CK2_PROVINCE_H
