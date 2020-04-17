#ifndef CK2_MODS_H
#define CK2_MODS_H

#include <map>
#include <optional>
#include <string>

class Configuration;
namespace CK2
{
	class Mods
	{
	public:
		Mods() = default;
		void loadModDirectory(const Configuration& theConfiguration);
		
		[[nodiscard]] std::optional<std::string> getModPath(const std::string& modName) const;
		[[nodiscard]] const auto& getMods() const { return usableMods; }

	private:
		void loadCK2ModDirectory(const Configuration& theConfiguration);

		[[nodiscard]] bool extractZip(const std::string& archive, const std::string& path) const;

		std::map<std::string, std::string> possibleMods; // name, path to mod directory 
		std::map<std::string, std::string> possibleCompressedMods; // name, path to zip file
		std::map<std::string, std::string> usableMods; // name, path (original or unpacked)
	};
}

#endif // CK2_MODS_H