#include "Titles.h"
#include "../../Mappers/ProvinceTitleMapper/ProvinceTitleMapper.h"
#include "../Characters/Characters.h"
#include "../Provinces/Province.h"
#include "../Provinces/Provinces.h"
#include "Liege.h"
#include "Log.h"
#include "ParserHelpers.h"
#include "Title.h"

CK2::Titles::Titles(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void CK2::Titles::registerKeys()
{
	registerRegex("[A-Za-z0-9-_]+", [this](const std::string& titleName, std::istream& theStream) {
		auto newTitle = std::make_shared<Title>(theStream, titleName);
		titles.insert(std::pair(newTitle->getName(), newTitle));
	});
	registerRegex("[A-Za-z0-9\\_:.-]+", commonItems::ignoreItem);
}

void CK2::Titles::linkHolders(const Characters& theCharacters)
{
	auto counter = 0;
	const auto& characters = theCharacters.getCharacters();
	for (const auto& title: titles)
	{
		if (title.second->getHolder().first)
		{
			const auto& charItr = characters.find(title.second->getHolder().first);
			if (charItr != characters.end())
			{
				title.second->setHolder(charItr->second);
				counter++;
			}
			else
			{
				Log(LogLevel::Warning) << "Holder ID: " << title.second->getHolder().first << " has no definition!";
			}
		}
	}
	Log(LogLevel::Info) << "<> " << counter << " title holders linked.";
}

void CK2::Titles::linkPreviousHolders(const Characters& theCharacters)
{
	auto counter = 0;
	const auto& characters = theCharacters.getCharacters();
	for (const auto& title: titles)
	{
		if (!title.second->getPreviousHolders().empty())
		{
			std::map<int, std::shared_ptr<Character>> previousHolders;
			for (const auto& previousHolder: title.second->getPreviousHolders())
			{
				const auto& charItr = characters.find(previousHolder.first);
				if (charItr != characters.end())
				{
					previousHolders.insert(std::pair(previousHolder.first, charItr->second));
					counter++;
				}
				else
				{
					Log(LogLevel::Warning) << "Previous Holder ID: " << previousHolder.first << " has no definition!";
				}
			}
			title.second->setPreviousHolders(previousHolders);
		}
	}
	Log(LogLevel::Info) << "<> " << counter << " previous title holders linked.";
}

void CK2::Titles::linkLiegePrimaryTitles()
{
	auto counterPrim = 0;
	auto counterBase = 0;
	auto counterDJPrim = 0;
	auto counterDJBase = 0;
	for (const auto& title: titles)
	{
		if (!title.second->getLiege().first.empty())
		{
			const auto& titleItr = titles.find(title.second->getLiege().first);
			if (titleItr != titles.end())
			{
				title.second->setLiegePrimaryTitle(titleItr->second);
				counterPrim++;
			}
			else
			{
				Log(LogLevel::Warning) << "Primary liege title ID: " << title.second->getLiege().first << " has no definition!";
			}
			if (!title.second->getLiege().second->getBaseTitle().first.empty())
			{
				const auto& title2Itr = titles.find(title.second->getLiege().second->getBaseTitle().first);
				if (title2Itr != titles.end())
				{
					title.second->setLiegeBaseTitle(title2Itr->second);
					counterBase++;
				}
				else
				{
					Log(LogLevel::Warning) << "Base liege title ID: " << title.second->getLiege().second->getBaseTitle().first << " has no definition!";
				}
			}
		}

		if (!title.second->getDeJureLiege().first.empty())
		{
			const auto& titleItr = titles.find(title.second->getDeJureLiege().first);
			if (titleItr != titles.end())
			{
				title.second->setDJLiegePrimaryTitle(titleItr->second);
				counterDJPrim++;
			}
			else
			{
				Log(LogLevel::Warning) << "Primary DJ liege title ID: " << title.second->getDeJureLiege().first << " has no definition!";
			}
			if (!title.second->getDeJureLiege().second->getBaseTitle().first.empty())
			{
				const auto& title2Itr = titles.find(title.second->getDeJureLiege().second->getBaseTitle().first);
				if (title2Itr != titles.end())
				{
					title.second->setDJLiegeBaseTitle(title2Itr->second);
					counterDJBase++;
				}
				else
				{
					Log(LogLevel::Warning) << "Base DJ liege title ID: " << title.second->getDeJureLiege().second->getBaseTitle().first << " has no definition!";
				}
			}
		}
	}
	Log(LogLevel::Info) << "<> " << counterPrim << " liege titles and " << counterBase << " liege base titles linked.";
	Log(LogLevel::Info) << "<> " << counterDJPrim << " dejure liege titles and " << counterDJBase << " dejure liege base titles linked.";
}

void CK2::Titles::linkVassals()
{
	auto counter = 0;
	auto counterDJ = 0;
	// We have title->liege links but not vice versa. The vice versa ones are more useful.
	for (const auto& title: titles)
	{
		if (title.second->getLiege().second) // At this point these links should all be set.
		{
			const auto& titleItr = titles.find(title.second->getLiege().first);
			if (titleItr != titles.end())
			{
				titleItr->second->registerVassal(std::pair(title.first, title.second));
				counter++;
			}
		}
		if (title.second->getDeJureLiege().second)
		{
			const auto& titleItr = titles.find(title.second->getDeJureLiege().first);
			if (titleItr != titles.end())
			{
				titleItr->second->registerDeJureVassal(std::pair(title.first, title.second));
				counterDJ++;
			}
		}
	}
	Log(LogLevel::Info) << "<> " << counter << " vassals and " << counterDJ << " de jure vassals linked.";
}

void CK2::Titles::linkProvinces(const Provinces& theProvinces, const mappers::ProvinceTitleMapper& provinceTitleMapper)
{
	// We're linking actual provinces to actual titles holding them (only c_level).
	// Conglomeration for higher-tier titles comes later.
	const auto& provinces = theProvinces.getProvinces();

	// reorganize provinces into searchable map
	std::map<std::string, std::shared_ptr<Province>> titleProvinceMap;
	for (const auto& province: provinces)
	{
		const auto& provinceTitle = provinceTitleMapper.getTitleForID(province.first);
		if (provinceTitle)
		{
			titleProvinceMap.insert(std::pair(*provinceTitle, province.second));
		}
		// We're not handling errors since c_wastelands won't have titles, but will be present.
	}

	// and map them to counties.
	auto counter = 0;
	for (const auto& title: titles)
	{
		const auto& titleName = title.first;
		// We could just look up directly in the map but then we'd miss potential errors,
		// so we check for c_ manually.
		if (titleName.find("c_") == 0)
		{
			const auto& provinceItr = titleProvinceMap.find(titleName);
			if (provinceItr != titleProvinceMap.end())
			{
				title.second->registerProvince(std::pair(provinceItr->second->getID(), provinceItr->second));
				title.second->registerDeJureProvince(std::pair(provinceItr->second->getID(), provinceItr->second));
				counter++;
			}
			else
			{
				Log(LogLevel::Warning) << "Title " << titleName << " has no province!";
			}
		}
	}
	Log(LogLevel::Info) << "<> " << counter << " provinces linked.";
}

void CK2::Titles::linkBaseTitles()
{
	// This is relevant for revolts, so we know where to merge them.
	auto counter = 0;
	auto counterBase = 0;
	for (const auto& title: titles)
	{
		if (!title.second->getBaseTitle().first.empty())
		{
			const auto& titleItr = titles.find(title.second->getBaseTitle().first);
			if (titleItr != titles.end())
			{
				title.second->setBaseTitleTitle(titleItr->second);
				counter++;
			}
			else
			{
				Log(LogLevel::Warning) << "Base title title ID: " << title.second->getBaseTitle().first << " has no definition!";
			}
			if (!title.second->getBaseTitle().second->getBaseTitle().first.empty())
			{
				const auto& title2Itr = titles.find(title.second->getBaseTitle().second->getBaseTitle().first);
				if (title2Itr != titles.end())
				{
					title.second->setBaseTitleBaseTitle(title2Itr->second);
					counterBase++;
				}
				else
				{
					Log(LogLevel::Warning) << "Base title base title ID: " << title.second->getBaseTitle().second->getBaseTitle().first << " has no definition!";
				}
			}
		}
	}
	Log(LogLevel::Info) << "<> " << counter << " base titles titles and " << counterBase << " base title base titles linked.";
}

void CK2::Titles::mergeRevolts()
{
	// major revolts need to have their leader drop the top-tier revolt title and relink
	// to revolt's base_title.

	std::set<std::string> droppedRevoltTitles;

	for (const auto& title: titles)
	{
		if (!title.second->isMajorRevolt())
			continue;
		// for a major revolt, scroll through all vassals and relink them to to base;
		for (const auto& vassal: title.second->getVassals())
		{
			const auto& revoltBaseTitle = title.second->getBaseTitle();
			vassal.second->overrideLiege(revoltBaseTitle);
			const auto& newLiege = vassal.second->getLiege().second->getTitle();
			newLiege.second->registerVassal(std::pair(vassal.first, vassal.second));
		}
		title.second->clearVassals();
		droppedRevoltTitles.insert(title.first);
	}
	// finally, clear them out.
	for (const auto& droppedRevolt: droppedRevoltTitles)
	{
		titles.erase(droppedRevolt);
	}
	Log(LogLevel::Info) << "<> " << droppedRevoltTitles.size() << " revolts merged.";
}
