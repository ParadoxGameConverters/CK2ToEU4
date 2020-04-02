#include "ProvinceTitleMapper.h"
#include "ParserHelpers.h"
#include <set>
#include "OSCompatibilityLayer.h"
#include "ProvinceTitleGrabber.h"

void mappers::ProvinceTitleMapper::loadProvinces(const std::string& CK2Path)
{
	// Goal of this mapper is to determine what c_title maps to what provinceID. It's not as trivial as it sounds.
	
	std::set<std::string> provinceFilenames;
	Utils::GetAllFilesInFolder(CK2Path + "/history/provinces", provinceFilenames);
	if (provinceFilenames.empty()) throw std::runtime_error(CK2Path + "/history/provinces is empty?");
	for (const auto& provinceFilename : provinceFilenames)
	{
		auto newProvince = ProvinceTitleGrabber(CK2Path + "/history/provinces/" + provinceFilename);

		// Patch for poor QA over there. Sorry future maintainers, but single provinceID can point to multiple c_titles,
		// as well as a single c_title can point to multiple provinceIDs. There's no way of knowing.
		if (getIDForTitle(newProvince.getTitle()))
		{
			provinceTitles[newProvince.getTitle()] = newProvince.getID();
		}
		else
		{
			provinceTitles.insert(std::make_pair(newProvince.getTitle(), newProvince.getID()));
		}
	}
	Log(LogLevel::Info) << ">> Loaded: " << provinceTitles.size() << " provinces from history.";
}

std::optional<int> mappers::ProvinceTitleMapper::getIDForTitle(const std::string& title)
{
	const auto& provItr = provinceTitles.find(title);
	if (provItr != provinceTitles.end()) return provItr->second;
	return std::nullopt;
}

std::optional<std::string> mappers::ProvinceTitleMapper::getTitleForID(int provID)
{
	std::vector<std::string> potentialTiles;
	for (const auto& province : provinceTitles) if (province.second == provID) potentialTiles.push_back(province.first);

	if (potentialTiles.size() > 1) Log(LogLevel::Warning) << "Province ID " << provID << " has multiple province titles defined! Filter this!";
	if (!potentialTiles.empty()) return *potentialTiles.begin();

	return std::nullopt;
}
