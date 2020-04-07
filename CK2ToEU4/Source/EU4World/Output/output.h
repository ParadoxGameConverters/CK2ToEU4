#ifndef OUTPUT_H
#define OUTPUT_H

#include "../../Mappers/VersionParser/VersionParser.h"
#include "../Country/Country.h"
#include "ModFile.h"
#include <ostream>

namespace EU4
{
std::ostream& operator<<(std::ostream& output, const Country& versionParser);
std::ostream& operator<<(std::ostream& output, const ModFile& modFile);
} // namespace EU4

namespace mappers
{
std::ostream& operator<<(std::ostream& output, const VersionParser& versionParser);
} // namespace mappers

#endif // OUTPUT_H