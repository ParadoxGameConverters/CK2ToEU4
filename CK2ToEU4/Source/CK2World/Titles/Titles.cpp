#include "Titles.h"
#include "Log.h"
#include "ParserHelpers.h"
#include "Title.h"
#include "../Characters/Characters.h"

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

void CK2::Titles::linkLiegePrimaryTitles()
{
	auto counterPrim = 0;
	auto counterBase = 0;
	auto counterDJPrim = 0;
	auto counterDJBase = 0;
	for (const auto& title : titles)
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
				Log(LogLevel::Warning) << "Primary liege title ID: " << titleItr->first << " has no definition!";
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
					Log(LogLevel::Warning) << "Base liege title ID: " << title2Itr->first << " has no definition!";
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
				Log(LogLevel::Warning) << "Primary DJ liege title ID: " << titleItr->first << " has no definition!";
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
					Log(LogLevel::Warning) << "Base DJ liege title ID: " << title2Itr->first << " has no definition!";
				}
			}
		}
	}
	Log(LogLevel::Info) << "<> " << counterPrim << " liege titles and " << counterBase << " liege base titles linked.";
	Log(LogLevel::Info) << "<> " << counterDJPrim << " liege DeJure titles and " << counterDJBase << " liege DeJure base titles linked.";
}
