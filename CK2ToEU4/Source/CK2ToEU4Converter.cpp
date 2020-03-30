#include "Configuration.h"
#include "Log.h"
#include "OSCompatibilityLayer.h"
#include <fstream>
#include "CK2ToEU4Converter.h"

void convertCK2ToEU4(const mappers::VersionParser& versionParser)
{
	ConfigurationFile configurationFile("configuration.txt");
	deleteExistingOutputFolder();

	//const CK2::World sourceWorld();
	//EU4::World destWorld(sourceWorld, versionParser);

	LOG(LogLevel::Info) << "* Conversion complete *";
}


void deleteExistingOutputFolder()
{
	const auto outputFolder = Utils::getCurrentDirectory() + "/output/" + theConfiguration.getOutputName();
	if (Utils::doesFolderExist(outputFolder))
	{
		if (!Utils::deleteFolder(outputFolder))
		{
			LOG(LogLevel::Error) << "Could not delete pre-existing output folder " << Utils::getCurrentDirectory() << "/output/" << theConfiguration.getOutputName();
			exit(-1);
		}
	}
}