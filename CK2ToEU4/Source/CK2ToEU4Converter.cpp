#include "Configuration/Configuration.h"
#include "Log.h"
#include "OSCompatibilityLayer.h"
#include <fstream>
#include "CK2ToEU4Converter.h"
#include "CK2World/World.h"

void convertCK2ToEU4(const mappers::VersionParser& versionParser)
{
	auto theConfiguration = std::make_shared<Configuration>();
	deleteExistingOutputFolder(theConfiguration->getOutputName());

	const CK2::World sourceWorld(theConfiguration);
	//EU4::World destWorld(sourceWorld, versionParser);

	LOG(LogLevel::Info) << "* Conversion complete *";
}


void deleteExistingOutputFolder(const std::string& outputName)
{
	const auto outputFolder = Utils::getCurrentDirectory() + "/output/" + outputName;
	if (Utils::doesFolderExist(outputFolder))
	{
		if (!Utils::deleteFolder(outputFolder))
		{
			LOG(LogLevel::Error) << "Could not delete pre-existing output folder " << Utils::getCurrentDirectory() << "/output/" << outputName;
			exit(-1);
		}
	}
}