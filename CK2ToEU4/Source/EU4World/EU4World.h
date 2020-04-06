#ifndef EU4_WORLD_H
#define EU4_WORLD_H
#include "../Mappers/ProvinceMapper/ProvinceMapper.h"
#include "../Mappers/TitleTagMapper/TitleTagMapper.h"

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
	class World
	{
	public:
		World(const CK2::World& sourceWorld, const Configuration& theConfiguration, const mappers::VersionParser& versionParser);

	private:
		void loadRegions(const Configuration& theConfiguration);

		mappers::ProvinceMapper provinceMapper;
		mappers::TitleTagMapper titleTagMapper;
	};
}

#endif // EU4_WORLD_H