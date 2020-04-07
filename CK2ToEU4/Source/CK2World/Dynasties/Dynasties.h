#ifndef CK2_DYNASTIES_H
#define CK2_DYNASTIES_H
#include "newParser.h"

namespace CK2
{
class Dynasty;
class Dynasties: commonItems::parser
{
  public:
	Dynasties() = default;
	Dynasties(std::istream& theStream);
	[[nodiscard]] const auto& getDynasties() const { return dynasties; }

  private:
	void registerKeys();

	std::map<int, std::shared_ptr<Dynasty>> dynasties;
};
} // namespace CK2

#endif // CK2_DYNASTIES_H
