#include "Characters.h"
#include "../../Mappers/PersonalityScraper/PersonalityScraper.h"
#include "../Dynasties/Dynasties.h"
#include "../Provinces/Province.h"
#include "../Provinces/Provinces.h"
#include "../Titles/Title.h"
#include "../Titles/Titles.h"
#include "Character.h"
#include "Log.h"
#include "ParserHelpers.h"

CK2::Characters::Characters(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void CK2::Characters::registerKeys()
{
	registerRegex("\\d+", [this](const std::string& charID, std::istream& theStream) {
		auto newCharacter = std::make_shared<Character>(theStream, std::stoi(charID));
		characters.insert(std::pair(newCharacter->getID(), newCharacter));
	});
	registerRegex("[A-Za-z0-9\\_:.-]+", commonItems::ignoreItem);
}

void CK2::Characters::linkDynasties(const Dynasties& theDynasties)
{
	auto counter = 0;
	const auto& dynasties = theDynasties.getDynasties();
	for (const auto& character: characters)
	{
		if (character.second->getDynasty().first)
		{
			const auto& dynastyItr = dynasties.find(character.second->getDynasty().first);
			if (dynastyItr != dynasties.end())
			{
				character.second->setDynasty(dynastyItr->second);
				counter++;
			}
			else
			{
				Log(LogLevel::Warning) << "Dynasty ID: " << character.second->getDynasty().first << " has no definition!";
			}
		}
	}
	Log(LogLevel::Info) << "<> " << counter << " dynasties linked.";
}

void CK2::Characters::linkLiegesAndSpouses()
{
	auto counterLiege = 0;
	auto counterSpouse = 0;
	for (const auto& character: characters)
	{
		if (character.second->getLiege().first)
		{
			const auto& characterItr = characters.find(character.second->getLiege().first);
			if (characterItr != characters.end())
			{
				character.second->setLiege(characterItr->second);
				counterLiege++;
			}
			else
			{
				Log(LogLevel::Warning) << "Liege ID: " << character.second->getLiege().first << " has no definition!";
			}
		}

		if (!character.second->getSpouses().empty())
		{
			std::map<int, std::shared_ptr<Character>> newSpouses;
			for (const auto& spouse: character.second->getSpouses())
			{
				const auto& characterItr = characters.find(spouse.first);
				if (characterItr != characters.end())
				{
					newSpouses.insert(std::pair(characterItr->first, characterItr->second));
					counterSpouse++;
				}
				else
				{
					Log(LogLevel::Warning) << "Spouse ID: " << spouse.first << " has no definition!";
				}
			}
			character.second->setSpouses(newSpouses);
		}
	}
	Log(LogLevel::Info) << "<> " << counterLiege << " lieges and " << counterSpouse << " spouses linked.";
}


void CK2::Characters::linkMothersAndFathers()
{
	auto counterMother = 0;
	auto counterFather = 0;
	for (const auto& character: characters)
	{
		if (character.second->getMother().first)
		{
			const auto& characterItr = characters.find(character.second->getMother().first);
			if (characterItr != characters.end())
			{
				character.second->setMother(std::pair(characterItr->first, characterItr->second));
				characterItr->second->registerChild(character);
				counterMother++;
			}
			else
			{
				Log(LogLevel::Warning) << "Mother ID: " << character.second->getMother().first << " has no definition!";
			}
		}

		if (character.second->getFather().first)
		{
			const auto& characterItr = characters.find(character.second->getFather().first);
			if (characterItr != characters.end())
			{
				character.second->setFather(std::pair(characterItr->first, characterItr->second));
				counterFather++;
				characterItr->second->registerChild(character);
			}
			else
			{
				Log(LogLevel::Warning) << "Father ID: " << character.second->getFather().first << " has no definition!";
			}
		}
	}
	Log(LogLevel::Info) << "<> " << counterMother << " mothers and " << counterFather << " fathers linked.";
}

void CK2::Characters::linkPrimaryTitles(const Titles& theTitles)
{
	auto counterPrim = 0;
	auto counterBase = 0;
	const auto& titles = theTitles.getTitles();
	for (const auto& character: characters)
	{
		if (!character.second->getPrimaryTitle().first.empty())
		{
			const auto& titleItr = titles.find(character.second->getPrimaryTitle().first);
			if (titleItr != titles.end())
			{
				character.second->setPrimaryTitle(titleItr->second);
				counterPrim++;
			}
			else
			{
				Log(LogLevel::Warning) << "Primary title ID: " << character.second->getPrimaryTitle().first
											  << " has no definition!";
			}
			if (!character.second->getPrimaryTitle().second->getBaseTitle().first.empty())
			{
				const auto& title2Itr = titles.find(character.second->getPrimaryTitle().second->getBaseTitle().first);
				if (title2Itr != titles.end())
				{
					character.second->setBaseTitle(title2Itr->second);
					counterBase++;
				}
				else
				{
					Log(LogLevel::Warning) << "Base title ID: "
												  << character.second->getPrimaryTitle().second->getBaseTitle().first
												  << " has no definition!";
				}
			}
		}
	}
	Log(LogLevel::Info) << "<> " << counterPrim << " primary titles and " << counterBase << " base titles linked.";
}

void CK2::Characters::linkCapitals(const Provinces& theProvinces)
{
	auto counterCapital = 0;
	const auto& provinces = theProvinces.getProvinces();

	// Extract all known baronies and linked provinces in a lookable place.
	std::map<std::string, std::pair<std::shared_ptr<Barony>, std::pair<int, std::shared_ptr<Province>>>> baronyMap;

	for (const auto& province: provinces)
	{
		const auto& baronies = province.second->getBaronies();
		for (const auto& barony: baronies)
		{
			baronyMap.insert(
				 std::pair(barony.first, std::pair(barony.second, std::pair(province.first, province.second))));
		}
	}

	for (const auto& character: characters)
	{
		if (!character.second->getCapital().first.empty())
		{
			const auto& baronyItr = baronyMap.find(character.second->getCapital().first);
			if (baronyItr != baronyMap.end())
			{
				character.second->setCapitalBarony(baronyItr->second.first);
				character.second->insertCapitalProvince(baronyItr->second.second);
				counterCapital++;
			}
			else
			{
				Log(LogLevel::Warning) << "Capital barony ID: " << character.second->getCapital().first
											  << " has no definition!";
			}
		}
	}
	Log(LogLevel::Info) << "<> " << counterCapital << " capital baronies linked.";
}

void CK2::Characters::assignPersonalities(const mappers::PersonalityScraper& personalityScraper)
{
	auto counter = 0;
	for (const auto& character: characters)
	{
		std::map<int, std::string> translatedTraits;
		for (const auto& trait: character.second->getTraits())
		{
			const auto& traitMatch = personalityScraper.getPersonalityForID(trait.first);
			if (traitMatch)
				translatedTraits.insert(std::pair(trait.first, *traitMatch));
		}
		counter += translatedTraits.size();
		character.second->setTraits(translatedTraits);
	}
	Log(LogLevel::Info) << "<> " << counter << " personalities observed.";
}
