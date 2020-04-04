#include "Provinces.h"
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
	registerRegex("\\d+", [this](const std::string& provID, std::istream& theStream) {
		auto newProvince = std::make_shared<Province>(theStream, std::stoi(provID));
		provinces.insert(std::pair(newProvince->getID(), newProvince));
		});
	registerRegex("[A-Za-z0-9\\_:.-]+", commonItems::ignoreItem);
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
				Log(LogLevel::Warning) << "Primary barony ID: " << province.second->getPrimarySettlement().first << " is not in the province: " << province.first << " !";
			}
		}
	}
	Log(LogLevel::Info) << "<> " << counter << " primary baronies linked.";
}
