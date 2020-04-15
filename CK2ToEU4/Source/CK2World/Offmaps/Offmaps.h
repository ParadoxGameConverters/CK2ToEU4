#ifndef CK2_OFFMAPS_H
#define CK2_OFFMAPS_H
#include "newParser.h"

namespace CK2
{
class Offmap;
class Offmaps: commonItems::parser
{
  public:
	Offmaps() = default;
	Offmaps(std::istream& theStream);

	[[nodiscard]] const auto& getOffmaps() const { return offmaps; }
	[[nodiscard]] std::optional<std::pair<int, std::shared_ptr<Offmap>>> getChina() const;

  private:
	void registerKeys();

	std::map<int, std::shared_ptr<Offmap>> offmaps;
};
} // namespace CK2

#endif // CK2_OFFMAPS_H
