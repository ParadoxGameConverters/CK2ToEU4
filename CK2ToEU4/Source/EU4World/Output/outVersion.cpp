#include "output.h"

std::ostream& mappers::operator<<(std::ostream& output, const VersionParser& versionParser)
{
	output << "Converter version " << versionParser.version << " \"" << versionParser.name << "\"\n";
	output << versionParser.descriptionLine << "\n";
	output << "Built on " << __TIMESTAMP__ << "\n";
	return output;
}
