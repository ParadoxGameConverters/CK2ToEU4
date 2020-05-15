#ifndef CK2_RELATION_H
#define CK2_RELATION_H
#include "Tributary.h"
#include "Parser.h"

namespace CK2
{
class Relation: commonItems::parser
{
  public:
	Relation() = default;
	Relation(std::istream& theStream, int second);

	void setFirst(const int first) { firstCharacterID = first; }

	[[nodiscard]] auto getFirst() const { return firstCharacterID; }
	[[nodiscard]] auto getSecond() const { return secondCharacterID; }
	[[nodiscard]] auto isFirstOverlord() const { return !tributary.getTributaryType().empty() && secondCharacterID == tributary.getTributaryID(); }
	[[nodiscard]] auto isSecondOverlord() const { return !tributary.getTributaryType().empty() && firstCharacterID == tributary.getTributaryID(); }
	[[nodiscard]] const auto& getTributaryType() const { return tributary.getTributaryType(); }

  private:
	void registerKeys();

	int firstCharacterID = 0;
	int secondCharacterID = 0;
	Tributary tributary;
};
} // namespace CK2

#endif // CK2_RELATION_H
