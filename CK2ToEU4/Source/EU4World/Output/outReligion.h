#ifndef EU4_OUT_RELIGION_H
#define EU4_OUT_RELIGION_H

#include "../EU4World.h"
#include <fstream>
#include "OSCompatibilityLayer.h"
#include <ostream>

namespace EU4
{
	class outReligion
	{
		public:
			outReligion(const Configuration& theConfiguration, std::vector<mappers::ReformedReligionMapping> unreligionReforms, std::vector<mappers::ReformedReligionMapping> religionReforms);

	};
} // namespace EU4


#endif // EU4_OUT_RELIGION_H