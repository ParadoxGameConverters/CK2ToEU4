#include "CK2ToEU4Converter.h"
#include "CK2World/World.h"
#include "Configuration/Configuration.h"
#include "EU4World/EU4World.h"
#include "Log.h"

void convertCK2ToEU4(const commonItems::ConverterVersion& converterVersion)
{
	Log(LogLevel::Progress) << "0 %";
	const auto theConfiguration = Configuration(converterVersion);
	const CK2::World sourceWorld(theConfiguration, converterVersion);
	EU4::World destWorld(sourceWorld, theConfiguration, converterVersion);

	Log(LogLevel::Notice) << "* Conversion complete *";
	Log(LogLevel::Progress) << "100 %";
}
