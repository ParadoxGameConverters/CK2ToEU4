#ifndef CK2_PROVINCE_H
#define CK2_PROVINCE_H
#include "Parser.h"

namespace CK2
{
class Title;
class Wonder;
class Barony;
class Province: commonItems::parser
{
  public:
	Province() = default;
	Province(std::istream& theStream, int provID);

	[[nodiscard]] const auto& getCulture() const { return culture; }
	[[nodiscard]] const auto& getReligion() const { return religion; }
	[[nodiscard]] const auto& getName() const { return name; }
	[[nodiscard]] const auto& getPrimarySettlement() const { return primarySettlement; }
	[[nodiscard]] const auto& getBaronies() const { return baronies; }
	[[nodiscard]] const auto& getTitle() const { return title; }
	[[nodiscard]] const auto& getWonder() const { return wonder; }

	[[nodiscard]] auto isDeJureHRE() const { return deJureHRE; }
	[[nodiscard]] auto getID() const { return provinceID; }
	[[nodiscard]] auto getMaxSettlements() const { return maxSettlements; }
	[[nodiscard]] auto getBaronyCount() const { return static_cast<int>(baronies.size()); }

	[[nodiscard]] int getBuildingWeight() const;

	void discardPrimarySettlement() { primarySettlement.first.clear(); }
	void setPrimarySettlement(std::shared_ptr<Barony> theBarony) { primarySettlement.second = std::move(theBarony); }
	void loadHoldingTitle(const std::pair<std::string, std::shared_ptr<Title>>& theTitle) { title = theTitle; }
	void loadWonder(const std::pair<int, std::shared_ptr<Wonder>>& theWonder) { wonder = theWonder; }
	void setDeJureHRE() { deJureHRE = true; }

  private:
	void registerKeys();

	bool deJureHRE = false;
	int provinceID = 0;
	int maxSettlements = 0;
	std::string culture;
	std::string religion;
	std::string name;
	std::pair<std::string, std::shared_ptr<Barony>> primarySettlement;
	std::pair<std::string, std::shared_ptr<Title>> title;
	std::pair<int, std::shared_ptr<Wonder>> wonder;
	std::map<std::string, std::shared_ptr<Barony>> baronies;
};
} // namespace CK2

#endif // CK2_PROVINCE_H
