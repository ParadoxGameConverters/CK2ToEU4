#include "Provinces.h"
#include "../Titles/Title.h"
#include "Log.h"
#include "ParserHelpers.h"
#include "Province.h"
#include "CommonRegexes.h"
#include "OSCompatibilityLayer.h"

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
	const mappers::MonumentsMapper monumentsMapper;
	for (const auto& wonder: wonders.getWonders())
	{
		if (wonder.second)
		{
			const auto& monumentName = wonder.second->getType();
			if (premadeMonuments.contains(monumentName))
				extantMonuments.emplace(monumentName);
			else if (wonder.second->getUpgrades().size() < 4)
			{					
				if (monumentName == "wonder_cathedral" || monumentName == "wonder_mosque" || monumentName == "wonder_synagogue" ||
					 monumentName == "wonder_temple_pagan" || monumentName == "wonder_temple_buddhist" || monumentName == "wonder_temple_hindu")
				{
					if (wonder.second->getUpgrades().size() < 2)
						wonder.second->addUpgrade("generic_religious_upgrade_1");
					if (wonder.second->getUpgrades().size() < 3)
						wonder.second->addUpgrade("generic_religious_upgrade_2");
					wonder.second->addUpgrade("generic_religious_upgrade_3");
				}
				else if (monumentName == "wonder_statue_ruler" || monumentName == "wonder_statue_horse")
				{
					if (wonder.second->getUpgrades().size() < 2)
						wonder.second->addUpgrade("generic_statue_upgrade_1");
					if (wonder.second->getUpgrades().size() < 3)
						wonder.second->addUpgrade("generic_statue_upgrade_2");
					wonder.second->addUpgrade("generic_statue_upgrade_3");
				}
				else if (monumentName == "wonder_fortress" || monumentName == "wonder_underground_city" || monumentName == "wonder_wall")
				{
					if (wonder.second->getUpgrades().size() < 2)
						wonder.second->addUpgrade("generic_fortification_upgrade_1");
					if (wonder.second->getUpgrades().size() < 3)
						wonder.second->addUpgrade("generic_fortification_upgrade_2");
					wonder.second->addUpgrade("generic_fortification_upgrade_3");
				}
				else if (monumentName == "wonder_harbor" || monumentName == "wonder_lighthouse")
				{
					if (wonder.second->getUpgrades().size() < 2)
						wonder.second->addUpgrade("generic_coastal_upgrade_1");
					if (wonder.second->getUpgrades().size() < 3)
						wonder.second->addUpgrade("generic_coastal_upgrade_2");
					wonder.second->addUpgrade("generic_coastal_upgrade_3");
				}
				else if (monumentName == "wonder_amphitheater" || monumentName == "wonder_palace" || monumentName == "wonder_garden")
				{
					if (wonder.second->getUpgrades().size() < 2)
						wonder.second->addUpgrade("generic_culture_upgrade_1");
					if (wonder.second->getUpgrades().size() < 3)
						wonder.second->addUpgrade("generic_culture_upgrade_2");
					wonder.second->addUpgrade("generic_culture_upgrade_3");
				}
				else if (monumentName == "wonder_university" || monumentName == "wonder_library")
				{
					if (wonder.second->getUpgrades().size() < 2)
						wonder.second->addUpgrade("generic_learning_upgrade_1");
					if (wonder.second->getUpgrades().size() < 3)
						wonder.second->addUpgrade("generic_learning_upgrade_2");
					wonder.second->addUpgrade("generic_learning_upgrade_3");
				}
				else if (monumentName == "wonder_mausoleum" || monumentName == "wonder_pyramid" || monumentName == "wonder_pagan_stones" ||
							monumentName == "wonder_aztec_pyramid")
				{
					if (wonder.second->getUpgrades().size() < 2)
						wonder.second->addUpgrade("generic_misc_upgrade_1");
					if (wonder.second->getUpgrades().size() < 3)
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
			// Fixing any blank names, using province/builder name
			if (wonder.second->getName().empty())
			{
				if (wonder.second->getType() == "wonder_cathedral")
					wonder.second->setName(provinceItr->second->getName() + " Cathedral");
				else if (wonder.second->getType() == "wonder_mosque")
					wonder.second->setName("Grand Mosque of " + provinceItr->second->getName());
				else if (wonder.second->getType() == "wonder_synagogue")
					wonder.second->setName("Great " + provinceItr->second->getName() + " Synagogue");
				else if (wonder.second->getType() == "wonder_temple_pagan" || wonder.second->getType() == "wonder_temple_buddhist" || wonder.second->getType() == "wonder_temple_hindu")
					wonder.second->setName("Great Temple of" + provinceItr->second->getName());
				else if (wonder.second->getType() == "wonder_statue_ruler")
					wonder.second->setName(characters.getCharacters().find(wonder.second->getBuilder())->second->getName() + " of " + characters.getCharacters().find(wonder.second->getBuilder())->second->getPrimaryTitle().second->getTitle().second->getDisplayName());
				else if (wonder.second->getType() == "wonder_statue_horse")
					wonder.second->setName("Colossal Mount of " + provinceItr->second->getName());
				else if (wonder.second->getType() == "wonder_fortress")
					wonder.second->setName("Fortress at " + provinceItr->second->getName());
				else if (wonder.second->getType() == "wonder_underground_city")
					wonder.second->setName("Underground City of " + provinceItr->second->getName());
				else if (wonder.second->getType() == "wonder_wall")
					wonder.second->setName("Great Wall of " + provinceItr->second->getName());
				else if (wonder.second->getType() == "wonder_harbor")
					wonder.second->setName("Grand Harbour of " + provinceItr->second->getName());
				else if (wonder.second->getType() == "wonder_lighthouse")
					wonder.second->setName("Lighthouse of " + provinceItr->second->getName());
				else if (wonder.second->getType() == "wonder_amphitheater")
					wonder.second->setName(provinceItr->second->getName() + " Colosseum");
				else if (wonder.second->getType() == "wonder_palace")
					wonder.second->setName("Glittering Palace of " + provinceItr->second->getName());
				else if (wonder.second->getType() == "wonder_garden")
					wonder.second->setName("Hanging Gardens of " + provinceItr->second->getName());
				else if (wonder.second->getType() == "wonder_university")
					wonder.second->setName(provinceItr->second->getName() + " University");
				else if (wonder.second->getType() == "wonder_library")
					wonder.second->setName("Library of " + provinceItr->second->getName());
				else if (wonder.second->getType() == "wonder_mausoleum")
					wonder.second->setName("Mausoleum at " + provinceItr->second->getName());
				else if (wonder.second->getType() == "wonder_pyramid")
					wonder.second->setName("Pyramids of " + characters.getCharacters().find(wonder.second->getBuilder())->second->getName());
				else if (wonder.second->getType() == "wonder_pagan_stones")
					wonder.second->setName("Standing Stones at " + provinceItr->second->getName());
				else if (wonder.second->getType() == "wonder_aztec_pyramid")
					wonder.second->setName(provinceItr->second->getName() + " Altar");
				else
					wonder.second->setName("Grand Monument at " + provinceItr->second->getName()); // Fallback
			}
			//Converts name to the proper encoding type
			wonder.second->setName(commonItems::convertWin1252ToUTF8(wonder.second->getName()));

			//Now we will finish building the monument
			if (!premadeMonuments.contains(monumentName))
				buildMonument(monumentsMapper, wonder.second);
		}
		counter++;
	}
	Log(LogLevel::Info) << "<> " << counter << " wonders have been linked.";
	return extantMonuments;
}


void CK2::Provinces::buildMonument(const mappers::MonumentsMapper& monumentsMapper, const std::shared_ptr<CK2::Wonder>& wonder)
{	
	wonder->setTrueDate(wonder->getBinaryDate());

	short numOfModifiers = 0;

	// Goes through each upgrade that a wonder has and creates vectors for the bonuses, only done for up to a max of 4 bonuses.
	for (const auto& upgrade: wonder->getUpgrades())
	{
		bool addedMod = false;

		if (const auto& upgItr = monumentsMapper.getWonders().find(upgrade); upgItr == monumentsMapper.getWonders().end())
		{
			Log(LogLevel::Warning) << "Upgrade " << upgrade << " has no mapping!";
			continue;
		}

		auto monumentsMapping = monumentsMapper.getWonders().find(upgrade)->second;

		if (monumentsMapping.getCanBeMoved())
			wonder->setCanBeMoved("yes");
		if (!monumentsMapping.getBuildTrigger().empty())
		{
			wonder->setBuildTrigger(monumentsMapping.getBuildTrigger());
			if (monumentsMapping.isOfBuilderCulture())
				wonder->setBuildTrigger(wonder->getBuildTrigger() + "AND = {\n\t\t\t\tculture = " + wonder->getBuilderCulture() + "\n\t\t\t\thas_owner_culture = yes\n\t\t\t}\n\t\t");
			if (monumentsMapping.isOfBuilderReligion())
				wonder->setBuildTrigger(wonder->getBuildTrigger() + "AND = {\n\t\t\t\treligion = " + wonder->getBuilderReligion() + "\n\t\t\t\thas_owner_religion = yes\n\t\t\t}\n\t\t");
		}

		for (const auto& mod: monumentsMapping.getProvinceModifiers())
		{
			if (!wonder->getProvinceModifiers().contains(mod.first))
			{
				wonder->addProvinceModifier(mod);
				addedMod = true;
			}
		}

		for (const auto& mod: monumentsMapping.getAreaModifiers())
		{
			if (!wonder->getAreaModifiers().contains(mod.first))
			{
				wonder->addAreaModifier(mod);
				addedMod = true;
			}
		}

		for (const auto& mod: monumentsMapping.getCountryModifiers())
		{
			if (!wonder->getCountryModifiers().contains(mod.first))
			{
				wonder->addCountryModifier(mod);
				addedMod = true;
			}
		}

		if (addedMod)
		{
			wonder->getOnUpgraded().emplace_back(monumentsMapping.getOnUpgraded()); // This way we will have 4 onUpgrades to match the 4 tiers
			numOfModifiers++;
		}
		if (numOfModifiers > 3)
			break;
	}
}