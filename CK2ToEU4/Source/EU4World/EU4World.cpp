#include "EU4World.h"
#include "../Configuration/Configuration.h"
#include "../CK2World/World.h"
#include "../Mappers/VersionParser/VersionParser.h"
#include <fstream>
#include <filesystem>
#include "Log.h"
#include "../CK2World/Titles/Title.h"

namespace fs = std::filesystem;

EU4::World::World(const CK2::World& sourceWorld, const Configuration& theConfiguration, const mappers::VersionParser& versionParser)
{
	provinceMapper.determineValidProvinces(theConfiguration);
	for (const auto& title: sourceWorld.getIndepTitles())
	{
		auto match = titleTagMapper.getTagForTitle(title.first, title.second->getBaseTitle().first, 0);
	}
}

/*
void EU4::World::loadRegions(const Configuration& theConfiguration)
{
	//LOG(LogLevel::Info) << "-> Loading Geographical Structure";
	loadRegions(theConfiguration);
	auto areaFilename = theConfiguration.getEU4Path() + "/map/area.txt";
	auto regionFilename = theConfiguration.getEU4Path() + "/map/region.txt";
	auto superRegionFilename = theConfiguration.getEU4Path() + "/map/superregion.txt";

	std::ifstream areaStream(fs::u8path(areaFilename));
	if (!areaStream.is_open()) throw std::runtime_error("Could not open map/area.txt!");
	//Areas installedAreas(areaStream);
	areaStream.close();
	//assignProvincesToAreas(installedAreas.getAreas());

	std::ifstream superRegionFile(fs::u8path(superRegionFilename));
	if (!superRegionFile.is_open()) throw std::runtime_error("Could not open map/superregion.txt!");
	//SuperRegions sRegions(superRegionFile);
	superRegionFile.close();

	std::ifstream regionStream(fs::u8path(regionFilename));
	if (!regionStream.is_open()) throw std::runtime_error("Could not open map/region.txt!");
	//regions = std::make_unique<Regions>(sRegions, installedAreas, regionStream);
	regionStream.close();
}
*/