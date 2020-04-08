#include "CK2ToEU4Converter.h"
#include "CK2World/World.h"
#include "Configuration/Configuration.h"
#include "EU4World/EU4World.h"
#include "Log.h"

void convertCK2ToEU4(const mappers::VersionParser& versionParser)
{
	const Configuration theConfiguration;
	const CK2::World sourceWorld(theConfiguration);
	EU4::World destWorld(sourceWorld, theConfiguration, versionParser);

	LOG(LogLevel::Info) << "* Conversion complete *";
}
