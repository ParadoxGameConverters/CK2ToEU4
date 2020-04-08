#include "outVersion.h"

std::ostream& mappers::operator<<(std::ostream& output, const VersionParser& versionParser)
{
	output << "\n\n";
	output << "************ -= The Paradox Converters Team =- ********************\n";
	output << "* Converter version " << versionParser.version << " \"" << versionParser.name << "\"\n";
	output << "* " << versionParser.descriptionLine << "\n";
	output << "* Built on " << __TIMESTAMP__ << "\n";
	output << "********************** + CK2 To EU4 + *****************************\n";
	return output;
}
