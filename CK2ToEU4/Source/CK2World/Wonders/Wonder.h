#ifndef CK2_WONDER_H
#define CK2_WONDER_H
#include "newParser.h"

namespace CK2
{
class Wonder: commonItems::parser
{
  public:
	Wonder() = default;
	Wonder(std::istream& theStream);

	[[nodiscard]] const auto& getName() const { return name; }
	[[nodiscard]] const auto& getType() const { return type; }
	[[nodiscard]] const auto& getStage() const { return stage; }
	[[nodiscard]] auto getProvinceID() const { return provinceID; }
	[[nodiscard]] auto isActive() const { return active; }
	[[nodiscard]] auto isTransferrable() const { return active && stage == 3; }
	[[nodiscard]] auto isSpent() const { return spent; }

	void setSpent() { spent = true; }
	
  private:
	void registerKeys();

	std::string type;
	int stage = 0;
	std::string name;
	int provinceID = 0;
	bool active = false;
	bool spent = false;
};
} // namespace CK2

#endif // CK2_WONDER_H
