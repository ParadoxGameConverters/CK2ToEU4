#include "Diplomacy.h"
#include "../../CK2World/Titles/Title.h"
#include "../Country/Country.h"
#include "Log.h"
#include "ParserHelpers.h"

EU4::Diplomacy::Diplomacy()
{
	LOG(LogLevel::Info) << "-> Loading Eastern Diplomacy";	
	registerKeys();
	parseFile("configurables/chinese_tributaries.txt");
	clearRegisteredKeywords();
	LOG(LogLevel::Info) << ">> " << agreements.size() << " tributaries loaded.";
}

void EU4::Diplomacy::registerKeys()
{
	registerKeyword("dependency", [this](const std::string& type, std::istream& theStream) {
		auto newAgreement = std::make_shared<Agreement>(theStream, type);
		agreements.emplace_back(newAgreement);
	});
	registerRegex("[a-zA-Z0-9\\_.:-]+", commonItems::ignoreItem);
}

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

			auto newAgreement = std::make_shared<Agreement>(country.first, targetTag.first, "vassal", country.second->getConversionDate());
			agreements.push_back(newAgreement);
		}
	}
}

void EU4::Diplomacy::updateTagsInAgreements(const std::string& oldTag, const std::string& newTag)
{
	for (const auto& agreement: agreements)
		agreement->updateTags(oldTag, newTag);
}