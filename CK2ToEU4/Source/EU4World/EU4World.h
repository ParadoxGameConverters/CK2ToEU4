#ifndef EU4_WORLD_H
#define EU4_WORLD_H
#include "../Mappers/ProvinceMapper/ProvinceMapper.h"
#include "../Mappers/TitleTagMapper/TitleTagMapper.h"
#include "Output/ModFile.h"

class Configuration;
namespace mappers
{
class VersionParser;
}

namespace CK2
{
class World;
}

namespace EU4
{
class Country;
class World
{
  public:
	World(const CK2::World& sourceWorld, const Configuration& theConfiguration, const mappers::VersionParser& versionParser);

  private:
	// void loadRegions(const Configuration& theConfiguration); waiting on geography.
	void importVanillaCountries(const std::string& eu4Path);
	void importCK2Countries(const CK2::World& sourceWorld);
	void output(const mappers::VersionParser& versionParser, const Configuration& theConfiguration) const;
	void createModFile(const Configuration& theConfiguration) const;
	void outputVersion(const mappers::VersionParser& versionParser, const Configuration& theConfiguration) const;
	void outputCommonCountriesFile(const Configuration& theConfiguration) const;
	void outputHistoryCountries(const Configuration& theConfiguration) const;
	void outputCommonCountries(const Configuration& theConfiguration) const;


	mappers::ProvinceMapper provinceMapper;
	mappers::TitleTagMapper titleTagMapper;
	ModFile modFile;
	std::map<std::string, std::shared_ptr<Country>> countries;
};
} // namespace EU4

#endif // EU4_WORLD_H