#include "EU4Province.h"
#include "../../CK2World/Provinces/Province.h"
#include "../../Mappers/TitleTagMapper/TitleTagMapper.h"

EU4::Province::Province(int id, const std::string& filePath): provID(id)
{
	// Load from a country file, if one exists. Otherwise rely on defaults.
	const auto startPos = filePath.find("/history");
	historyProvincesFile = filePath.substr(startPos + 1, filePath.length() - startPos);
	details = ProvinceDetails(filePath);
}

void EU4::Province::initializeFromCK2(std::shared_ptr<CK2::Province> origProvince, mappers::TitleTagMapper& titletagMapper)
{
	srcProvince = std::move(origProvince);
	const auto& ownerMatch = titletagMapper.getTagForTitle(srcProvince->getTitle().first);
	if (ownerMatch) details.owner = *ownerMatch;
	details.controller = details.owner;
	
	// History section
}
