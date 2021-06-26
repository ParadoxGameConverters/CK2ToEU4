#include "outVersion.h"
#include <fstream>

std::ostream& mappers::operator<<(std::ostream& output, const ConverterVersion& converterVersion)
{
	output << "\n\n";
	output << "************ -= The Paradox Converters Team =- ********************\n";
	output << "* Converter version " << converterVersion.version << " \"" << converterVersion.name << "\"\n";
	output << "* " << converterVersion.getDescription() << "\n";
	output << "* Built on " << __TIMESTAMP__ << "\n";
	output << "********************** + CK2 To EU4 + *****************************\n";
	return output;
}
