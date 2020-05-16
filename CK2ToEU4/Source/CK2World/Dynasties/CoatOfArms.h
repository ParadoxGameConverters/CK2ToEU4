#ifndef CK2_COAT_OF_ARMS_H
#define CK2_COAT_OF_ARMS_H
#include "Parser.h"

namespace CK2
{
class CoatOfArms: commonItems::parser
{
  public:
	CoatOfArms() = default;
	explicit CoatOfArms(std::istream& theStream);

	[[nodiscard]] const auto& getReligion() const { return religion; }

  private:
	void registerKeys();

	std::string religion;
};
} // namespace CK2

#endif // CK2_COAT_OF_ARMS_H
