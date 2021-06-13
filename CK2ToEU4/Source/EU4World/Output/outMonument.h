#ifndef OUT_MONUMENT_H
#define OUT_MONUMENT_H

#include "../../CK2World/Wonders/Wonder.h"
#include "../../CK2World/World.h"
#include "../EU4World.h"
#include "OSCompatibilityLayer.h"

namespace EU4
{
class outMonument
{
  public:
	outMonument(const Configuration& theConfiguration, std::optional<std::pair<int, std::shared_ptr<CK2::Wonder>>> wonder,
		 int eu4Province);																							 // Dynamic Monuments
	outMonument(const Configuration& theConfiguration, const std::set<std::string>& premades); // Premade Monuments

	const std::string gfxType(const std::string& base); // Decides which wonder gfx the monument will use.
};
}; // namespace EU4


#endif // OUT_MONUMENT_H