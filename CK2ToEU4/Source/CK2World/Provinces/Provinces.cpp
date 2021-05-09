#include "Provinces.h"
#include "../Wonders/Wonder.h"
#include "../Wonders/Wonders.h"
#include "../../Mappers/MonumentsMapper/MonumentsMapping.h"
#include "Log.h"
#include "ParserHelpers.h"
#include "Province.h"

CK2::Provinces::Provinces(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void CK2::Provinces::registerKeys()
{
	registerRegex(R"(\d+)", [this](const std::string& provID, std::istream& theStream) {
		auto newProvince = std::make_shared<Province>(theStream, std::stoi(provID));
		provinces.insert(std::pair(newProvince->getID(), newProvince));
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}

void CK2::Provinces::linkPrimarySettlements()
{
	auto counter = 0;
	for (const auto& province: provinces)
	{
		if (province.second->getPrimarySettlement().first == "---" || province.second->getPrimarySettlement().first.empty())
		{
			province.second->discardPrimarySettlement();
		}
		else
		{
			const auto& baronies = province.second->getBaronies();
			const auto& baronyItr = baronies.find(province.second->getPrimarySettlement().first);
			if (baronyItr != baronies.end())
			{
				province.second->setPrimarySettlement(baronyItr->second);
				counter++;
			}
			else
			{
				Log(LogLevel::Warning) << "Primary barony ID: " << province.second->getPrimarySettlement().first << " is not in the province: " << province.first
											  << " !";
			}
		}
	}
	Log(LogLevel::Info) << "<> " << counter << " primary baronies linked.";
}

void CK2::Provinces::linkWonders(const Wonders& wonders) //No Leviathan DLC
{	
	auto counter = 0;
	for (const auto& wonder: wonders.getWonders())
	{
		if (!wonder.second->isTransferrable())
			continue;
		const auto& provinceItr = provinces.find(wonder.second->getProvinceID());
		if (provinceItr == provinces.end())
		{
			Log(LogLevel::Warning) << "Wonder " << wonder.first << " is in province " << wonder.second->getProvinceID() << " which doesn't exist?";
			continue;
		}
		provinceItr->second->loadWonder(wonder);
		counter++;
	}
	Log(LogLevel::Info) << "<> " << counter << " active and finished wonders have been linked.";
}
std::set<std::string> CK2::Provinces::linkMonuments(const Wonders& wonders, Characters characters) // Leviathan DLC
{
	auto counter = 0;
	std::set<std::string> premadeMonuments = { "wonder_pyramid_giza", "wonder_pagan_stones_stonehenge" "wonder_mausoleum_halicarnassus", "wonder_lighthouse_alexandria",
											   "wonder_temple_hindu_konark", "wonder_apostolic_palace", "wonder_house_of_wisdom", "wonder_underground_city_petra",
											   "wonder_cathedral_hagia_sophia", "wonder_cathedral_notre_dame" //Theses monuments have set definitions already
	};
	std::set<std::string> existentMonuments;
	for (const auto& wonder: wonders.getWonders())
	{
		if (premadeMonuments.count(wonder.second->getType()))
			existentMonuments.emplace(premadeMonuments.count(wonder.second->getType()));		
		else if (wonder.second->getStage() < 3)
		{
			if (wonder.second->getType() == "wonder_cathedral" || wonder.second->getType() == "wonder_mosque" || wonder.second->getType() == "wonder_synagogue" ||
				wonder.second->getType() == "wonder_temple_pagan" || wonder.second->getType() == "wonder_temple_buddhist" || wonder.second->getType() == "wonder_temple_hindu")
			{
				if (wonder.second->getStage() < 1)
					wonder.second->addUpgrade("generic_religious_upgrade_1");
				if (wonder.second->getStage() < 2)
					wonder.second->addUpgrade("generic_religious_upgrade_2");
				wonder.second->addUpgrade("generic_religious_upgrade_3");
			}
			else if (wonder.second->getType() == "wonder_statue_ruler" || wonder.second->getType() == "wonder_statue_horse")
			{
				if (wonder.second->getStage() < 1)
					wonder.second->addUpgrade("generic_statue_upgrade_1");
				if (wonder.second->getStage() < 2)
					wonder.second->addUpgrade("generic_statue_upgrade_2");
				wonder.second->addUpgrade("generic_statue_upgrade_3");
			}
			else if (wonder.second->getType() == "wonder_fortress" || wonder.second->getType() == "wonder_underground_city" || wonder.second->getType() == "wonder_wall")
			{
				if (wonder.second->getStage() < 1)
					wonder.second->addUpgrade("generic_fortification_upgrade_1");
				if (wonder.second->getStage() < 2)
					wonder.second->addUpgrade("generic_fortification_upgrade_2");
				wonder.second->addUpgrade("generic_fortification_upgrade_3");
			}
			else if (wonder.second->getType() == "wonder_harbor" || wonder.second->getType() == "wonder_lighthouse")
			{
				if (wonder.second->getStage() < 1)
					wonder.second->addUpgrade("generic_coastal_upgrade_1");
				if (wonder.second->getStage() < 2)
					wonder.second->addUpgrade("generic_coastal_upgrade_2");
				wonder.second->addUpgrade("generic_coastal_upgrade_3");
			}
			else if (wonder.second->getType() == "wonder_amphitheater" || wonder.second->getType() == "wonder_palace" || wonder.second->getType() == "wonder_garden")
			{
				if (wonder.second->getStage() < 1)
					wonder.second->addUpgrade("generic_coastal_upgrade_1");
				if (wonder.second->getStage() < 2)
					wonder.second->addUpgrade("generic_coastal_upgrade_2");
				wonder.second->addUpgrade("generic_coastal_upgrade_3");
			}
			else if (wonder.second->getType() == "wonder_university" || wonder.second->getType() == "wonder_library")
			{
				if (wonder.second->getStage() < 1)
					wonder.second->addUpgrade("generic_learning_upgrade_1");
				if (wonder.second->getStage() < 2)
					wonder.second->addUpgrade("generic_learning_upgrade_2");
				wonder.second->addUpgrade("generic_learning_upgrade_3");
			}
			else if (wonder.second->getType() == "wonder_mausoleum" || wonder.second->getType() == "wonder_pyramid" ||
					 wonder.second->getType() == "wonder_pagan_stones" || wonder.second->getType() == "wonder_aztec_pyramid")
			{
				if (wonder.second->getStage() < 1)
					wonder.second->addUpgrade("generic_misc_upgrade_1");
				if (wonder.second->getStage() < 2)
					wonder.second->addUpgrade("generic_misc_upgrade_2");
				wonder.second->addUpgrade("generic_misc_upgrade_3");
			}
		}

		wonder.second->setBuilderCulture(characters.getCharacters().find(wonder.second->getBuilder())->second->getCulture());
		wonder.second->setBuilderReligion(characters.getCharacters().find(wonder.second->getBuilder())->second->getReligion());

		const auto& provinceItr = provinces.find(wonder.second->getProvinceID());
		if (provinceItr == provinces.end())
		{
			Log(LogLevel::Warning) << "Wonder " << wonder.first << " is in province " << wonder.second->getProvinceID() << " which doesn't exist?";
			continue;
		}
		provinceItr->second->loadMonument(wonder);
		counter++;
	}
	Log(LogLevel::Info) << "<> " << counter << " active and finished wonders have been linked.";
	return existentMonuments;
}
