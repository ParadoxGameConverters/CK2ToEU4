#ifndef CK2_WONDER_H
#define CK2_WONDER_H
#include "Parser.h"
#include <set>

namespace CK2
{
class Wonder: commonItems::parser
{
  public:
	Wonder() = default;
	explicit Wonder(std::istream& theStream);

	[[nodiscard]] const auto& getType() const { return type; }
	[[nodiscard]] const auto& getName() const { return name; }
	[[nodiscard]] const auto& getDesc() const { return desc; }
	[[nodiscard]] auto getWonderID() const { return wonderID; }
	[[nodiscard]] auto getStage() const { return stage; }
	[[nodiscard]] auto getProvinceID() const { return provinceID; }
	[[nodiscard]] auto getBuilder() const { return builderID; }
	[[nodiscard]] auto getBinaryDate() const { return binaryDate; }
	[[nodiscard]] auto isActive() const { return active; }
	[[nodiscard]] auto isTransferrable() const { return active && stage == 3; }
	[[nodiscard]] auto getTrueDate() { return trueDate; }
	[[nodiscard]] auto getBuilderCulture() { return builderCulture; }
	[[nodiscard]] auto getBuilderReligion() { return builderReligion; }
	[[nodiscard]] auto getUpgrades() { return upgrades; }
	[[nodiscard]] auto isSpent() const { return spent; }
	void addUpgrade(const std::string& mod){ upgrades.emplace(mod); }
	void setWonderID(int mod) { wonderID = mod; }
	void setTrueDate(int binDate);
	void setBuilderCulture(const std::string& culture) { builderCulture = culture; }
	void setBuilderReligion(const std::string& religion) { builderReligion = religion; }
	void setSpent() { spent = true; }

  private:
	void registerKeys();

	int wonderID = -1;
	int stage = 0;
	int provinceID = 0;
	int builderID = 0;
	int binaryDate = 0;
	bool active = false;
	bool spent = false;
	std::string type;
	std::string name;
	std::string desc;
	std::string trueDate = "0";
	std::string builderCulture;
	std::string builderReligion;

	std::set<std::string> upgrades;
};
} // namespace CK2

#endif // CK2_WONDER_H
