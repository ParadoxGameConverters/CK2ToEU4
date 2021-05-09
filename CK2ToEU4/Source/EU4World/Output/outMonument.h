#ifndef OUT_MONUMENT_H
#define OUT_MONUMENT_H

#include "../../CK2World/World.h"
#include "../EU4World.h"
#include "../../CK2World/Provinces/Province.h"
#include "../../CK2World/Wonders/Wonder.h"
#include <fstream>
#include "OSCompatibilityLayer.h"
#include <ostream>

namespace EU4 
{
	class outMonument
	{
		public:
			outMonument(const Configuration& theConfiguration, std::optional<std::pair<int, std::shared_ptr<CK2::Wonder>>> wonder); //Dynamic Monuments
			outMonument(const Configuration theConfiguration, std::set<std::string> premades); //Premade Monuments
	};
}; // namespace outReligion


#endif // OUT_MONUMENT_H