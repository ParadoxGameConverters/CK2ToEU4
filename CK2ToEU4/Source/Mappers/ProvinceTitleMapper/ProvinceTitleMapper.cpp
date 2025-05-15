#include "ProvinceTitleMapper.h"
#include "../../CK2World/Provinces/Provinces.h"
#include "../../CK2World/Titles/Titles.h"
#include "OSCompatibilityLayer.h"
#include "ProvinceTitleGrabber.h"
#include <set>

void mappers::ProvinceTitleMapper::loadProvinces(const std::filesystem::path& CK2Path)
{
	// Goal of this mapper is to determine what c_title maps to what provinceID. It's not as trivial as it sounds.

	auto provinceFilenames = commonItems::GetAllFilesInFolder(CK2Path / "history/provinces");
	if (provinceFilenames.empty())
		throw std::runtime_error(CK2Path.string() + "/history/provinces is empty?");
	for (const auto& provinceFilename: provinceFilenames)
	{
		if (provinceFilename.extension() != ".txt")
			continue;
		auto newProvince = ProvinceTitleGrabber(CK2Path / "history/provinces" / provinceFilename);
		if (!newProvince.getID())
			continue;

		// At this stage, single provinceID can point to multiple c_titles, as well as a single
		// c_title can point to multiple provinceIDs. We must filter this before we can use it!
		origProvinceTitles.insert(std::make_pair(newProvince.getID(), newProvince.getTitle()));
	}
	Log(LogLevel::Info) << ">> Loaded: " << origProvinceTitles.size() << " provinces from history.";
}

void mappers::ProvinceTitleMapper::updateProvinces(const std::filesystem::path& path)
{
	for (const auto& provinceFilename: commonItems::GetAllFilesInFolder(path / "history/provinces"))
	{
		if (provinceFilename.extension() != ".txt")
			continue;
		auto newProvince = ProvinceTitleGrabber(path / "history/provinces" / provinceFilename);
		if (!newProvince.getID() || newProvince.getTitle().empty())
			continue;

		std::multimap<int, std::string> replacementTitles;
		for (const auto& [id, title]: origProvinceTitles)
			if (id != newProvince.getID() && title != newProvince.getTitle())
				replacementTitles.insert(std::pair(id, title));
		replacementTitles.insert(std::make_pair(newProvince.getID(), newProvince.getTitle()));
		origProvinceTitles = std::move(replacementTitles);
	}
	Log(LogLevel::Info) << ">> Loaded: " << origProvinceTitles.size() << " provinces from history.";
}

std::optional<int> mappers::ProvinceTitleMapper::getIDForTitle(const std::string& title) const
{
	for (const auto& province: provinceTitles)
		if (province.second == title)
			return province.first;
	return std::nullopt;
}

std::optional<std::string> mappers::ProvinceTitleMapper::getTitleForID(int provID) const
{
	const auto& provItr = provinceTitles.find(provID);
	if (provItr != provinceTitles.end())
		return provItr->second;
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

	std::map<int, std::string> newProvinceTitles;
	std::set<int> newProvinceIDs;
	std::set<std::string> newTitles;

	for (const auto& provinceTitle: origProvinceTitles)
	{
		if (knownTitles.contains(provinceTitle.second) && knownProvinceIDs.contains(provinceTitle.first) && !newProvinceIDs.contains(provinceTitle.first) &&
			 !newTitles.contains(provinceTitle.second))
		{
			newProvinceTitles.insert(std::pair(provinceTitle.first, provinceTitle.second));
			newProvinceIDs.insert(provinceTitle.first);
			newTitles.insert(provinceTitle.second);
		}
	}
	Log(LogLevel::Info) << "<> Dropped " << origProvinceTitles.size() - newProvinceTitles.size() << " invalid mappings.";
	provinceTitles.swap(newProvinceTitles);
}
