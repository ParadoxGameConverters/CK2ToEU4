#ifndef OUTPUT_H
#define OUTPUT_H

#include <ostream>
#include "../../Mappers/VersionParser/VersionParser.h"

namespace EU4
{
}

namespace mappers
{
	std::ostream& operator<<(std::ostream& output, const VersionParser& versionParser);
}

#endif // OUTPUT_H