#include "Provinces.h"
#include "../Wonders/Wonder.h"
#include "../Wonders/Wonders.h"
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
		if (province.second->getPrimarySettlement().first == "---" ||
			 province.second->getPrimarySettlement().first.empty())
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
				Log(LogLevel::Warning) << "Primary barony ID: " << province.second->getPrimarySettlement().first
											  << " is not in the province: " << province.first << " !";
			}
		}
	}
	Log(LogLevel::Info) << "<> " << counter << " primary baronies linked.";
}

void CK2::Provinces::linkWonders(const Wonders& wonders)
{
	auto counter = 0;
	for (const auto& wonder: wonders.getWonders())
	{
		if (!wonder.second->isTransferrable())
			continue;
		const auto& provinceItr = provinces.find(wonder.second->getProvinceID());
		if (provinceItr == provinces.end())
		{
			Log(LogLevel::Warning) << "Wonder " << wonder.first << " is in province " << wonder.second->getProvinceID()
										  << " which doesn't exist?";
			continue;
		}
		provinceItr->second->loadWonder(wonder);
		counter++;
	}
	Log(LogLevel::Info) << "<> " << counter << " active and finished wonders have been linked.";
}
