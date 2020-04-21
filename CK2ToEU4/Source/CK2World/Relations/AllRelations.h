#ifndef CK2_DIPLOMACY_H
#define CK2_DIPLOMACY_H
#include "Relations.h"
#include "newParser.h"

namespace CK2
{
class Diplomacy: commonItems::parser
{
  public:
	Diplomacy() = default;
	explicit Diplomacy(std::istream& theStream);

	[[nodiscard]] const auto& getDiplomacy() const { return diplomacy; }

  private:
	void registerKeys();
	std::map<int, Relations> diplomacy; // characterID, their relations
};
} // namespace CK2

#endif // CK2_DIPLOMACY_H
