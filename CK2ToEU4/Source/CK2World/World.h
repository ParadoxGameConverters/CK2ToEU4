#ifndef CK2_WORLD_H
#define CK2_WORLD_H
#include "GameVersion.h"
#include "../Mappers/IAmHreMapper/IAmHreMapper.h"
#include "../Mappers/ShatterEmpiresMapper/ShatterEmpiresMapper.h"
#include "../Mappers/PersonalityScraper/PersonalityScraper.h"
#include "../Mappers/ProvinceTitleMapper/ProvinceTitleMapper.h"
#include "Characters/Characters.h"
#include "Date.h"
#include "Dynasties/Dynasties.h"
#include "Mods/Mods.h"
#include "Offmaps/Offmaps.h"
#include "Provinces/Province.h"
#include "Provinces/Provinces.h"
#include "Relations/AllRelations.h"
#include "Titles/Liege.h"
#include "Titles/Titles.h"
#include "Vars/Vars.h"
#include "Wonders/Wonders.h"
#include "Parser.h"

class Configuration;

namespace CK2
{
class World: commonItems::parser
{
  public:
	explicit World(const Configuration& theConfiguration);

	[[nodiscard]] const auto& getProvinceTitleMapper() const { return provinceTitleMapper; }
	[[nodiscard]] const auto& getIndepTitles() const { return independentTitles; }
	[[nodiscard]] const auto& getProvinces() const { return provinces.getProvinces(); }
	[[nodiscard]] const auto& getConversionDate() const { return endDate; }
	[[nodiscard]] const auto& getOffmaps() const { return offmaps; }
	[[nodiscard]] const auto& getDiplomacy() const { return diplomacy; }
	[[nodiscard]] const auto& getVars() const { return vars; }
	[[nodiscard]] const auto& getMods() const { return mods; }
	[[nodiscard]] auto isInvasion() const { return (invasion); }

  private:
	bool uncompressSave(const std::string& saveGamePath);
	void alterSunset(const Configuration& theConfiguration);
	void verifySave(const std::string& saveGamePath);
	void filterIndependentTitles();
	void mergeIndependentBaronies() const;
	void congregateProvinces();
	void congregateDeJureProvinces();
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
	void linkElectors();
	void loadDynasties(const Configuration& theConfiguration);
	void loadProvinces(const Configuration& theConfiguration);

	bool invasion = false;
	date endDate = date("1444.11.11");
	date startDate = date("1.1.1");
	GameVersion CK2Version;

	struct saveData
	{
		bool compressed = false;
		std::string metadata;
		std::string gamestate;
	};
	saveData saveGame;

	Provinces provinces;
	Characters characters;
	Titles titles;
	Dynasties dynasties;
	Wonders wonders;
	Offmaps offmaps;
	Mods mods;
	Diplomacy diplomacy;
	Vars vars;
	mappers::ShatterEmpiresMapper shatterEmpiresMapper;
	mappers::IAmHreMapper iAmHreMapper;
	mappers::PersonalityScraper personalityScraper;
	mappers::ProvinceTitleMapper provinceTitleMapper;
	std::map<std::string, std::shared_ptr<Title>> independentTitles;
	std::map<std::string, Liege> dynamicTitles; // Reusing Liege as it has identical structure
};
} // namespace CK2

#endif // CK2_WORLD_H
