#include "LocalizationMapper.h"
#include "../../Configuration/Configuration.h"
#include "Log.h"
#include "OSCompatibilityLayer.h"
#include <fstream>
#include <set>

void mappers::LocalizationMapper::scrapeLocalizations(const Configuration& theConfiguration, const Mods& mods)
{
	LOG(LogLevel::Info) << "-> Reading Words";
	auto filenames = commonItems::GetAllFilesInFolder(theConfiguration.getCK2Path() + "/localisation/");
	for (const auto& file: filenames)
	{
		std::ifstream theFile(theConfiguration.getCK2Path() + "/localisation/" + file);
		scrapeStream(theFile);
		theFile.close();
	}
	for (const auto& mod: mods)
	{
		if (commonItems::DoesFolderExist(mod.path + "/localisation/"))
		{
			Log(LogLevel::Info) << "\t>> Found some words in [" << mod.name << "]: " << mod.path + "/localization/";
			filenames = commonItems::GetAllFilesInFolder(mod.path + "/localisation/");
			for (const auto& file: filenames)
			{
				if (file.find(".csv") == std::string::npos) continue;
				std::ifstream theFile(mod.path + "/localisation/" + file);
				scrapeStream(theFile);
				theFile.close();
			}			
		}
	}
	
	// Override with our keys
	if (commonItems::DoesFileExist("configurables/ck2_localization_override.csv"))
	{
		std::ifstream theFile("configurables/ck2_localization_override.csv");
		scrapeStream(theFile);
		theFile.close();
	}
	LOG(LogLevel::Info) << ">> " << localizations.size() << " words read.";
}

void mappers::LocalizationMapper::scrapeStream(std::istream& theStream)
{
	while (!theStream.eof())
	{
		std::string line;
		getline(theStream, line);

		if (line.length() < 4 || line [0] == '#' || line[1] == '#' )
			continue;

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
		if (localizations.count(key))
			localizations[key] = newBlock;
		else
			localizations.insert(std::pair(key, newBlock));
	}
}


std::optional<mappers::LocBlock> mappers::LocalizationMapper::getLocBlockForKey(const std::string& key) const
{
	const auto& keyItr = localizations.find(key);
	if (keyItr == localizations.end())
		return std::nullopt;

	if (!keyItr->second.english.empty() && (keyItr->second.spanish.empty() || keyItr->second.german.empty() || keyItr->second.french.empty()))
	{
		auto newBlock = keyItr->second;
		if (newBlock.spanish.empty())
			newBlock.spanish = newBlock.english;
		if (newBlock.german.empty())
			newBlock.german = newBlock.english;
		if (newBlock.french.empty())
			newBlock.french = newBlock.english;
		return std::move(newBlock);
	}
	// either all is well, or we're missing english. Can't do anything about the latter.
	return keyItr->second;
}