#ifndef CK2_WORLD_H
#define CK2_WORLD_H
#include "../Common/Version.h"
#include "../Mappers/IAmHreMapper/IAmHreMapper.h"
#include "../Mappers/ProvinceTitleMapper/ProvinceTitleMapper.h"
#include "../Mappers/PersonalityScraper/PersonalityScraper.h"
#include "Characters/Characters.h"
#include "Date.h"
#include "Dynasties/Dynasties.h"
#include "Provinces/Province.h"
#include "Provinces/Provinces.h"
#include "Titles/Liege.h"
#include "Titles/Titles.h"
#include "Wonders/Wonders.h"
#include "Offmaps/Offmaps.h"
#include "newParser.h"
#include "Mods/Mods.h"

class Configuration;

namespace CK2
{
class World: commonItems::parser
{
  public:
	World(const Configuration& theConfiguration);
	[[nodiscard]] const auto& getProvinceTitleMapper() const { return provinceTitleMapper; }
	[[nodiscard]] const auto& getIndepTitles() const { return independentTitles; }
	[[nodiscard]] const auto& getProvinces() const { return provinces.getProvinces(); }
	[[nodiscard]] const auto& getConversionDate() const { return endDate; }
	[[nodiscard]] const auto& getOffmaps() const { return offmaps; }
	[[nodiscard]] auto isInvasion() const { return invasion; }

  private:
	void verifySave(const std::string& saveGamePath);
	bool uncompressSave(const std::string& saveGamePath);
	void filterIndependentTitles();
	void mergeIndependentBaronies() const;
	void congregateProvinces();
	void sanityCheckifyProvinces();
	void shatterHRE(const Configuration& theConfiguration) const;
	void shatterEmpires(const Configuration& theConfiguration) const;
	void filterProvincelessTitles();
	void splitVassals();
	void gatherCourtierNames();
	void determineHeirs();
	void resolvePrimogeniture(const std::string& genderLaw, const std::pair<int, std::shared_ptr<Character>>& holder) const;
	void resolveUltimogeniture(const std::string& genderLaw, const std::pair<int, std::shared_ptr<Character>>& holder) const;
	void resolveTanistry(const std::string& genderLaw, const std::pair<int, std::shared_ptr<Character>>& holder) const;
	void resolveTurkish(const std::pair<int, std::shared_ptr<Character>>& holder) const;
	void linkCelestialEmperor() const;
	void verifyReligionsAndCultures(const Configuration& theConfiguration);
	void loadDynastiesFromMods(const Configuration& theConfiguration);

	date endDate = date("1444.11.11");
	date startDate = date("1.1.1");
	Version CK2Version;

	struct saveData {
		bool compressed = false;
		std::string metadata;
		std::string gamestate;
	};
	saveData saveGame;

	mappers::ProvinceTitleMapper provinceTitleMapper;
	Provinces provinces;
	Characters characters;
	Titles titles;
	Dynasties dynasties;
	Wonders wonders;
	Offmaps offmaps;
	mappers::IAmHreMapper iAmHreMapper;
	mappers::PersonalityScraper personalityScraper;
	Mods mods;
	std::map<std::string, Liege> dynamicTitles; // Reusing Liege as it has identical structure

	std::map<std::string, std::shared_ptr<Title>> independentTitles;
	bool invasion = false;
};
} // namespace CK2

#endif // CK2_WORLD_H
