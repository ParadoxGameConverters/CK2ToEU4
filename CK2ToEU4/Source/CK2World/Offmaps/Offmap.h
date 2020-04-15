#ifndef CK2_OFFMAP_H
#define CK2_OFFMAP_H
#include "newParser.h"

namespace CK2
{
class Character;
class Offmap: commonItems::parser
{
  public:
	Offmap() = default;
	Offmap(std::istream& theStream);

	[[nodiscard]] const auto& getName() const { return name; }
	[[nodiscard]] const auto& getType() const { return type; }
	[[nodiscard]] const auto& getHolder() const { return holder; }

	void setHolder(const std::pair<int, std::shared_ptr<Character>>& theHolder) { holder = theHolder; }
	
  private:
	void registerKeys();
	std::string type;
	std::pair<int, std::shared_ptr<Character>> holder;
	std::string name;
};
} // namespace CK2

#endif // CK2_OFFMAP_H
