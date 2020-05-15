#include "Mods.h"
#include "../../Configuration/Configuration.h"
#include "Log.h"
#include "Mod.h"
#include "OSCompatibilityLayer.h"
#include "ZipFile.h"
#include <filesystem>
#include <fstream>
#include <set>
#include <stdexcept>
#include <string>
namespace fs = std::filesystem;

void CK2::Mods::loadModDirectory(const Configuration& theConfiguration)
{
	loadCK2ModDirectory(theConfiguration);

	Log(LogLevel::Info) << "\tDetermining Mod Usability";
	auto allMods = possibleMods;
	allMods.insert(possibleCompressedMods.begin(), possibleCompressedMods.end());
	for (const auto& usedMod: allMods)
	{
		auto possibleModPath = getModPath(usedMod.first);
		if (possibleModPath)
		{
			if (!Utils::DoesFolderExist(*possibleModPath) && !Utils::DoesFileExist(*possibleModPath))
			{
				Log(LogLevel::Warning) << usedMod.first + " could not be found in the specified mod directory " +
														"- a valid mod directory must be specified. Tried " + *possibleModPath;
				continue;
			}

			LOG(LogLevel::Info) << "\t\t->> Found potentially usable CK2 Mod: " << *possibleModPath;
			usableMods.insert(std::pair(usedMod.first, *possibleModPath));
		}
		else
		{
			Log(LogLevel::Warning)
				 << "No path could be found for " + usedMod.first +
						  ". Check that the mod is present and that the .mod file specifies the path for the mod";
		}
	}
}

void CK2::Mods::loadCK2ModDirectory(const Configuration& theConfiguration)
{
	const auto& CK2ModsPath = theConfiguration.getCK2ModsPath();
	if (!Utils::DoesFolderExist(CK2ModsPath))
		throw std::invalid_argument(
			 "No Crusader Kings 2 mods directory was specified in configuration.txt, or the path was invalid!");

	LOG(LogLevel::Info) << "\tCK2 mods directory is " << CK2ModsPath;

	auto filenames = Utils::GetAllFilesInFolder(CK2ModsPath);
	for (const auto& filename: filenames)
	{
		if (fs::path(filename).extension() != ".mod")
			continue;
		try
		{
			std::ifstream modFile(fs::u8path(CK2ModsPath + "/" + filename));
			Mod theMod(modFile);
			modFile.close();

			if (theMod.looksValid())
			{
				if (!theMod.isCompressed())
				{
					std::string recordDirectory;
					Log(LogLevel::Debug) << "checking dir:" << CK2ModsPath + "/" + theMod.getPath();
					if (Utils::DoesFolderExist(CK2ModsPath + "/" + theMod.getPath()))
					{
						recordDirectory = CK2ModsPath + "/" + theMod.getPath();
					}
					else
					{
						throw std::invalid_argument("");
					}

					possibleMods.insert(std::make_pair(theMod.getName(), recordDirectory));
					Log(LogLevel::Info) << "\t\tFound potential mod named " << theMod.getName() << " with a mod file at "
											  << CK2ModsPath << "/mod/" + filename << " and itself at " << recordDirectory;
				}
				else
				{
					std::string recordDirectory;
					if (Utils::DoesFileExist(CK2ModsPath + "/" + theMod.getPath()))
					{
						recordDirectory = CK2ModsPath + "/" + theMod.getPath();
					}
					else
					{
						throw std::invalid_argument("");
					}

					possibleCompressedMods.insert(std::make_pair(theMod.getName(), recordDirectory));
					Log(LogLevel::Info) << "\t\tFound a compessed mod named " << theMod.getName() << " with a mod file at "
											  << CK2ModsPath << "/" << filename << " and itself at " << recordDirectory;
				}
			}
			else
			{
				Log(LogLevel::Warning) << "Mod at " << CK2ModsPath + "/" + filename << " does not look valid.";
			}
		}
		catch (std::exception&)
		{
			LOG(LogLevel::Warning) << "Error while reading " << CK2ModsPath << "/" << filename
										  << ". "
											  "Mod will not be useable for conversions.";
		}
	}
}

std::optional<std::string> CK2::Mods::getModPath(const std::string& modName) const
{
	const auto& mod = possibleMods.find(modName);
	if (mod != possibleMods.end())
	{
		return mod->second;
	}

	const auto& compressedMod = possibleCompressedMods.find(modName);
	if (compressedMod != possibleCompressedMods.end())
	{
		const auto archivePath = compressedMod->second;
		auto uncompressedName = archivePath.substr(0, archivePath.find_last_of('.'));
		auto pos = uncompressedName.find_last_of('\\');
		if (pos != std::string::npos)
		{
			uncompressedName = uncompressedName.substr(pos + 1, uncompressedName.size());
		}
		pos = uncompressedName.find_last_of('/');
		if (pos != std::string::npos)
		{
			uncompressedName = uncompressedName.substr(pos + 1, uncompressedName.size());
		}

		Utils::TryCreateFolder("mods/");

		if (!Utils::DoesFolderExist("mods/" + uncompressedName))
		{
			LOG(LogLevel::Info) << "\t\tUncompressing: " << archivePath;
			if (!extractZip(archivePath, "mods/" + uncompressedName))
			{
				LOG(LogLevel::Warning) << "We have trouble automatically uncompressing your mod.";
				LOG(LogLevel::Warning) << "Please, manually uncompress: " << archivePath;
				LOG(LogLevel::Warning) << "Into: CK2ToEU4/mods/" << uncompressedName;
				LOG(LogLevel::Warning) << "Then run the converter again. Thank you and good luck.";
				return std::nullopt;
			}
		}

		if (Utils::DoesFolderExist("mods/" + uncompressedName))
		{
			return "mods/" + uncompressedName;
		}
	}

	return std::nullopt;
}

bool CK2::Mods::extractZip(const std::string& archive, const std::string& path) const
{
	Utils::TryCreateFolder(path);
	auto modfile = ZipFile::Open(archive);
	if (!modfile)
		return false;
	for (size_t entryNum = 0; entryNum < modfile->GetEntriesCount(); ++entryNum)
	{
		const auto& entry = modfile->GetEntry(entryNum);
		const auto& inpath = entry->GetFullName();
		const auto& name = entry->GetName();
		if (entry->IsDirectory())
			continue;

		// Does target directory exist?
		const auto dirnamepos = inpath.find(name);
		const auto dirname = path + "/" + inpath.substr(0, dirnamepos);
		if (!Utils::DoesFolderExist(dirname))
		{
			// we need to craft our way through to target directory.
			auto remainder = inpath;
			auto currentpath = path;
			while (remainder != name)
			{
				const auto pos = remainder.find_first_of('/');
				if (pos != std::string::npos)
				{
					auto makedirname = remainder.substr(0, pos);
					currentpath += "/" + makedirname;
					Utils::TryCreateFolder(currentpath);
					remainder = remainder.substr(pos + 1, remainder.length());
				}
				else
					break;
			}
		}
		ZipFile::ExtractFile(archive, inpath, path + "/" + inpath);
	}
	return true;
}