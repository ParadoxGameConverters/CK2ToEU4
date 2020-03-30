#ifndef CK2TOEU4_CONVERTER_H
#define CK2TOEU4_CONVERTER_H
#include "Mappers/VersionParser/VersionParser.h"

namespace mappers {
	class VersionParser;
}

void convertCK2ToEU4(const mappers::VersionParser& versionParser);
void deleteExistingOutputFolder();

#endif // CK2TOEU4_CONVERTER_H
