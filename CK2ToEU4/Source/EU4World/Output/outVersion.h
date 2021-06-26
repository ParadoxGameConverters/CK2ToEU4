#ifndef OUT_VERSION_H
#define OUT_VERSION_H

#include "../../Mappers/ConverterVersion/ConverterVersion.h"

namespace mappers
{
std::ostream& operator<<(std::ostream& output, const ConverterVersion& converterVersion);
} // namespace mappers

#endif // OUT_VERSION_H