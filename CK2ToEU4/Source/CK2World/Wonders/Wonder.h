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
	[[nodiscard]] auto hasBase() const { return base; }
	[[nodiscard]] auto isTransferrable() const { return active && stage == 3; }
	[[nodiscard]] const auto& getTrueDate() const { return trueDate; }
	[[nodiscard]] const auto& getCanBeMoved() const { return canBeMoved; }
	[[nodiscard]] const auto& getBuilderCulture() const { return builderCulture; }
	[[nodiscard]] const auto& getBuilderReligion() const { return builderReligion; }
	[[nodiscard]] const auto& getBuildTrigger() const { return buildTrigger; }
	[[nodiscard]] const auto& getOnUpgraded() const { return onUpgraded; }
	[[nodiscard]] const auto& getProvinceModifiers() const { return provinceModifiers; }
	[[nodiscard]] const auto& getAreaModifiers() const { return areaModifiers; }
	[[nodiscard]] const auto& getCountryModifiers() const { return countryModifiers; }
	[[nodiscard]] const auto& getUpgrades() const { return upgrades; }
	[[nodiscard]] auto isSpent() const { return spent; }

	void addUpgrade(const std::string& mod) { upgrades.emplace(mod); }
	void setWonderID(int mod) { wonderID = mod; }
	void setName(const std::string& newName) { name = newName; }
	void setTrueDate(int binDate);
	void setBase(const bool isBase) { base = isBase; }
	void setCanBeMoved(const std::string& can) { canBeMoved = can; }
	void setBuilderCulture(const std::string& culture) { builderCulture = culture; }
	void setBuilderReligion(const std::string& religion) { builderReligion = religion; }
	void setBuildTrigger(const std::string& trigger) { buildTrigger = trigger; }
	void addProvinceModifier(const std::pair<std::string, std::vector<double>>& mod) { provinceModifiers.emplace(mod); }
	void addAreaModifier(const std::pair<std::string, std::vector<double>>& mod) { areaModifiers.emplace(mod); }
	void addCountryModifier(const std::pair<std::string, std::vector<double>>& mod) { countryModifiers.emplace(mod); }
	void addOnUpgraded(const std::string& on) { onUpgraded.emplace_back(on); }
	void setSpent() { spent = true; }

  private:
	void registerKeys();

	int wonderID = -1;
	int stage = 0;
	int provinceID = 0;
	int builderID = 0;
	int binaryDate = 0;
	bool base = false;
	bool active = false;
	bool spent = false;
	std::string type;
	std::string name;
	std::string desc;
	std::string trueDate = "0";
	std::string canBeMoved = "no";
	std::string builderCulture;
	std::string builderReligion;
	std::string buildTrigger;

	std::set<std::string> upgrades;
	std::vector<std::string> onUpgraded;
	std::map<std::string, std::vector<double>> provinceModifiers;
	std::map<std::string, std::vector<double>> areaModifiers;
	std::map<std::string, std::vector<double>> countryModifiers;
};
} // namespace CK2

#endif // CK2_WONDER_H
