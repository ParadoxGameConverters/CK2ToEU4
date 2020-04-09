#include "LocalizationMapper.h"
#include "Log.h"
#include "../../Configuration/Configuration.h"
#include <set>
#include "OSCompatibilityLayer.h"
#include <fstream>

void mappers::LocalizationMapper::scrapeLocalizations(const Configuration& theConfiguration)
{
	LOG(LogLevel::Info) << "-> Reading Words";
	std::set<std::string> filenames;
	Utils::GetAllFilesInFolder(theConfiguration.getCK2Path() + "/localisation/", filenames);
	for (const auto& file: filenames) { std::ifstream theFile(theConfiguration.getCK2Path() + "/localisation/" + file);
		while (!theFile.eof()) {
			std::string line;
			getline(theFile, line);

			if (line[0] == '#' || line.length() < 4) continue;

			auto sepLoc = line.find_first_of(';');
			auto key = line.substr(0, sepLoc);
			auto newLine = line.substr(sepLoc + 1, line.length());
			sepLoc = newLine.find_first_of(';');
			auto english = newLine.substr(0, sepLoc);
			newLine = newLine.substr(sepLoc + 1, newLine.length());
			sepLoc = newLine.find_first_of(';');
			auto french = newLine.substr(0, sepLoc);
			newLine = newLine.substr(sepLoc + 1, newLine.length());
			sepLoc = newLine.find_first_of(';');
			auto german = newLine.substr(0, sepLoc);
			newLine = newLine.substr(sepLoc + 1, newLine.length());
			sepLoc = newLine.find_first_of(';');
			newLine = newLine.substr(sepLoc + 1, newLine.length());
			sepLoc = newLine.find_first_of(';');
			auto spanish = newLine.substr(0, sepLoc);

			LocBlock newBlock;
			newBlock.english = english;
			newBlock.french = french;
			newBlock.german = german;
			newBlock.spanish = spanish;
			localizations.insert(std::pair(key, newBlock));
		}
		theFile.close();
	}
	LOG(LogLevel::Info) << ">> " << localizations.size() << " words read.";
}


std::optional<mappers::LocBlock> mappers::LocalizationMapper::getLocBlockForKey(const std::string& key) const
{
	const auto& keyItr = localizations.find(key);
	if (keyItr != localizations.end()) return keyItr->second;
	return std::nullopt;
}