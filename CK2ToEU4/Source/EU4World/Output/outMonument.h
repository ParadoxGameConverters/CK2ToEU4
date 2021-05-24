#ifndef OUT_MONUMENT_H
#define OUT_MONUMENT_H

#include "../EU4World.h"
#include "../../CK2World/World.h"
#include "../../CK2World/Provinces/Province.h"
#include "../../CK2World/Wonders/Wonder.h"
#include "../../Configuration/Configuration.h"
#include <fstream>
#include "OSCompatibilityLayer.h"

namespace EU4 
{
	class outMonument
	{
		public:
			outMonument(const Configuration& theConfiguration, std::optional<std::pair<int, std::shared_ptr<CK2::Wonder>>> wonder); //Dynamic Monuments
			outMonument(const Configuration& theConfiguration, const std::set<std::string>& premades); //Premade Monuments
	};
}; // namespace outReligion


#endif // OUT_MONUMENT_H