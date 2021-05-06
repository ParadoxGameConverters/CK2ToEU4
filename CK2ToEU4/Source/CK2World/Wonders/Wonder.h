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

	void fillConstructionHistory(std::istream& theStream);

	[[nodiscard]] const auto& getName() const { return name; }
	[[nodiscard]] const auto& getType() const { return type; }
	[[nodiscard]] const auto& getStage() const { return stage; }
	[[nodiscard]] auto getProvinceID() const { return provinceID; }
	[[nodiscard]] auto getDate() const { return date; }
	[[nodiscard]] auto isActive() const { return active; }
	[[nodiscard]] auto isTransferrable() const { return active && stage == 3; }
	[[nodiscard]] auto getTrueDate() { return trueDate; }
	void setTrueDate(std::string mod) { trueDate = mod; }
	void setTrueDate(int mod);

  private:
	void registerKeys();

	short stage = 0;
	int provinceID = 0;
	int date = 0;
	bool active = false;
	std::string type;
	std::string name;
	std::string desc;
	std::string trueDate = "0";

	std::set<std::string> upgrades;
};
} // namespace CK2

#endif // CK2_WONDER_H
