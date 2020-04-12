#include "Diplomacy.h"
#include "../../CK2World/Titles/Title.h"
#include "../Country/Country.h"
#include "Log.h"

void EU4::Diplomacy::importAgreements(const std::map<std::string, std::shared_ptr<Country>>& countries)
{
	LOG(LogLevel::Info) << "-> Explaining Diplomacy Like It's Five";
	// Vassalages
	for (const auto& country: countries) {
		if (country.second->getTitle().first.empty()) continue;
		for (const auto& targetTitle: country.second->getTitle().second->getGeneratedVassals()) {
			const auto& targetTag = targetTitle.second->getEU4Tag();
			if (targetTag.first.empty()) continue;
			if (targetTag.second->getProvinces().empty()) continue;

			auto newAgreement = Agreement(country.first, targetTag.first, "vassal", country.second->getConversionDate());
			agreements.push_back(newAgreement);
		}
	}
}
