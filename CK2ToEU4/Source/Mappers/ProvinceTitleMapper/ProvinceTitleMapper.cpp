#include "ProvinceTitleMapper.h"
#include "../../CK2World/Provinces/Provinces.h"
#include "../../CK2World/Titles/Titles.h"
#include "OSCompatibilityLayer.h"
#include "ParserHelpers.h"
#include "ProvinceTitleGrabber.h"
#include <set>

void mappers::ProvinceTitleMapper::loadProvinces(const std::string& CK2Path)
{
	// Goal of this mapper is to determine what c_title maps to what provinceID. It's not as trivial as it sounds.

	auto provinceFilenames = commonItems::GetAllFilesInFolder(CK2Path + "/history/provinces");
	if (provinceFilenames.empty())
		throw std::runtime_error(CK2Path + "/history/provinces is empty?");
	for (const auto& provinceFilename: provinceFilenames)
	{
		if (provinceFilename.find(".txt") == std::string::npos)
			continue;
		auto newProvince = ProvinceTitleGrabber(CK2Path + "/history/provinces/" + provinceFilename);
		if (!newProvince.getID())
			continue;
		
		// At this stage, single provinceID can point to multiple c_titles, as well as a single
		// c_title can point to multiple provinceIDs. We must filter this before we can use it!
		origProvinceTitles.insert(std::make_pair(newProvince.getTitle(), newProvince.getID()));
	}
	Log(LogLevel::Info) << ">> Loaded: " << origProvinceTitles.size() << " provinces from history.";
}

std::optional<int> mappers::ProvinceTitleMapper::getIDForTitle(const std::string& title) const
{
	const auto& provItr = provinceTitles.find(title);
	if (provItr != provinceTitles.end())
		return provItr->second;
	return std::nullopt;
}

std::optional<std::string> mappers::ProvinceTitleMapper::getTitleForID(int provID) const
{
	for (const auto& province: provinceTitles)
		if (province.second == provID)
			return province.first;
	return std::nullopt;
}

void mappers::ProvinceTitleMapper::filterSelf(const CK2::Provinces& theProvinces, const CK2::Titles& theTitles)
{
	// This function's purpose is to filter out invalid provinceID-title mappings from /history/provinces.

	std::set<int> knownProvinceIDs;
	std::set<std::string> knownTitles;

	for (const auto& province: theProvinces.getProvinces())
	{
		knownProvinceIDs.insert(province.first);
	}
	for (const auto& title: theTitles.getTitles())
	{
		knownTitles.insert(title.first);
	}

	std::map<std::string, int> newProvinceTitles;

	for (const auto& provinceTitle: origProvinceTitles)
	{
		if (knownTitles.count(provinceTitle.first) && knownProvinceIDs.count(provinceTitle.second))
		{
			newProvinceTitles.insert(std::pair(provinceTitle.first, provinceTitle.second));
		}
	}
	Log(LogLevel::Info) << "<> Dropped " << origProvinceTitles.size() - newProvinceTitles.size() << " invalid mappings.";
	provinceTitles.swap(newProvinceTitles);
}
