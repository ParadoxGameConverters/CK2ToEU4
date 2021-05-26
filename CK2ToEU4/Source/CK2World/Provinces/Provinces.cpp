#include "Provinces.h"
#include "../Wonders/Wonder.h"
#include "../Wonders/Wonders.h"
#include "../../Mappers/MonumentsMapper/MonumentsMapping.h"
#include "Log.h"
#include "ParserHelpers.h"
#include "Province.h"
#include "CommonRegexes.h"

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
		if (wonder.second && !wonder.second->isTransferrable())
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
std::set<std::string> CK2::Provinces::linkMonuments(const Wonders& wonders, const Characters& characters) // Leviathan DLC
{
	auto counter = 0;
	const std::set<std::string> premadeMonuments = { "wonder_pyramid_giza", "wonder_pagan_stones_stonehenge", "wonder_mausoleum_halicarnassus", "wonder_lighthouse_alexandria",
											   "wonder_temple_hindu_konark", "wonder_apostolic_palace", "wonder_house_of_wisdom", "wonder_underground_city_petra",
											   "wonder_cathedral_hagia_sophia", "wonder_cathedral_notre_dame" //Theses monuments have set definitions already
	};
	std::set<std::string> extantMonuments;
	for (const auto& wonder: wonders.getWonders())
	{
		if (wonder.second)
		{			
			if (const auto& monumentName = wonder.second->getType(); premadeMonuments.contains(monumentName))
				extantMonuments.emplace(monumentName);
			else if (wonder.second->getStage() < 3)
			{
				if (monumentName == "wonder_cathedral" || monumentName == "wonder_mosque" || monumentName == "wonder_synagogue" ||
					 monumentName == "wonder_temple_pagan" || monumentName == "wonder_temple_buddhist" || monumentName == "wonder_temple_hindu")
				{
					if (wonder.second->getStage() < 1)
						wonder.second->addUpgrade("generic_religious_upgrade_1");
					if (wonder.second->getStage() < 2)
						wonder.second->addUpgrade("generic_religious_upgrade_2");
					wonder.second->addUpgrade("generic_religious_upgrade_3");
				}
				else if (monumentName == "wonder_statue_ruler" || monumentName == "wonder_statue_horse")
				{
					if (wonder.second->getStage() < 1)
						wonder.second->addUpgrade("generic_statue_upgrade_1");
					if (wonder.second->getStage() < 2)
						wonder.second->addUpgrade("generic_statue_upgrade_2");
					wonder.second->addUpgrade("generic_statue_upgrade_3");
				}
				else if (monumentName == "wonder_fortress" || monumentName == "wonder_underground_city" || monumentName == "wonder_wall")
				{
					if (wonder.second->getStage() < 1)
						wonder.second->addUpgrade("generic_fortification_upgrade_1");
					if (wonder.second->getStage() < 2)
						wonder.second->addUpgrade("generic_fortification_upgrade_2");
					wonder.second->addUpgrade("generic_fortification_upgrade_3");
				}
				else if (monumentName == "wonder_harbor" || monumentName == "wonder_lighthouse")
				{
					if (wonder.second->getStage() < 1)
						wonder.second->addUpgrade("generic_coastal_upgrade_1");
					if (wonder.second->getStage() < 2)
						wonder.second->addUpgrade("generic_coastal_upgrade_2");
					wonder.second->addUpgrade("generic_coastal_upgrade_3");
				}
				else if (monumentName == "wonder_amphitheater" || monumentName == "wonder_palace" || monumentName == "wonder_garden")
				{
					if (wonder.second->getStage() < 1)
						wonder.second->addUpgrade("generic_coastal_upgrade_1");
					if (wonder.second->getStage() < 2)
						wonder.second->addUpgrade("generic_coastal_upgrade_2");
					wonder.second->addUpgrade("generic_coastal_upgrade_3");
				}
				else if (monumentName == "wonder_university" || monumentName == "wonder_library")
				{
					if (wonder.second->getStage() < 1)
						wonder.second->addUpgrade("generic_learning_upgrade_1");
					if (wonder.second->getStage() < 2)
						wonder.second->addUpgrade("generic_learning_upgrade_2");
					wonder.second->addUpgrade("generic_learning_upgrade_3");
				}
				else if (monumentName == "wonder_mausoleum" || monumentName == "wonder_pyramid" || monumentName == "wonder_pagan_stones" ||
							monumentName == "wonder_aztec_pyramid")
				{
					if (wonder.second->getStage() < 1)
						wonder.second->addUpgrade("generic_misc_upgrade_1");
					if (wonder.second->getStage() < 2)
						wonder.second->addUpgrade("generic_misc_upgrade_2");
					wonder.second->addUpgrade("generic_misc_upgrade_3");
				}
			}

			if (wonder.second->getBuilder() > 0)
			{
				if (const auto& builder = characters.getCharacters().find(wonder.second->getBuilder()); builder != characters.getCharacters().end())
				{
					wonder.second->setBuilderCulture(builder->second->getCulture());
					wonder.second->setBuilderReligion(builder->second->getReligion());
				}				
			}

			const auto& provinceItr = provinces.find(wonder.second->getProvinceID());
			if (provinceItr == provinces.end())
			{
				Log(LogLevel::Warning) << "Wonder " << wonder.first << " is in province " << wonder.second->getProvinceID() << " which doesn't exist?";
				continue;
			}
			provinceItr->second->loadMonument(wonder);
		}
		counter++;
	}
	Log(LogLevel::Info) << "<> " << counter << " wonders have been linked.";
	return extantMonuments;
}
