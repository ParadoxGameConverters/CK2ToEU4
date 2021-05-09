#ifndef CK2_WORLD_H
#define CK2_WORLD_H
#include "GameVersion.h"
#include "../Mappers/IAmHreMapper/IAmHreMapper.h"
#include "../Mappers/ShatterEmpiresMapper/ShatterEmpiresMapper.h"
#include "../Mappers/PersonalityScraper/PersonalityScraper.h"
#include "../Mappers/ProvinceTitleMapper/ProvinceTitleMapper.h"
#include "../Mappers/ReformedReligionMapper/ReformedReligionMapper.h"
#include "../Mappers/MonumentsMapper/MonumentsMapper.h"
#include "../Mappers/ReligionMapper/ReligionMapper.h"
#include "Characters/Characters.h"
#include "Date.h"
#include "Dynasties/Dynasties.h"
#include "Flags/Flags.h"
#include "Mods/Mods.h"
#include "Offmaps/Offmaps.h"
#include "Provinces/Province.h"
#include "Provinces/Provinces.h"
#include "Relations/AllRelations.h"
#include "Religions/Religions.h"
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
	[[nodiscard]] const auto& getConversionDate() const { return endDate; }
	[[nodiscard]] const auto& getOffmaps() const { return offmaps; }
	[[nodiscard]] const auto& getCharacters() const { return characters; }
	[[nodiscard]] const auto& getDiplomacy() const { return diplomacy; }
	[[nodiscard]] const auto& getVars() const { return vars; }
	[[nodiscard]] const auto& getMods() const { return mods; }
	[[nodiscard]] const auto& getProvinces() const { return provinces.getProvinces(); }
	[[nodiscard]] const auto& getReligionReforms() const { return religionReforms; }
	[[nodiscard]] const auto& getUnreligionReforms() const { return unreligionReforms; }
	[[nodiscard]] const auto& getExistentPremadeMonuments() const { return existentPremadeMonuments; }
	[[nodiscard]] auto isInvasion() const { return invasion; }
	[[nodiscard]] auto wasNoReformation() const { return wereNoReformations; }
	[[nodiscard]] auto isGreekReformation() const { return greekReformation; }

	void setExistentPremadeMonuments(std::set<std::string> premades) { existentPremadeMonuments = premades; }

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
	void flagHREProvinces(const Configuration& theConfiguration);
	void shatterEmpires(const Configuration& theConfiguration) const;
	void filterProvincelessTitles();
	void splitVassals(const Configuration& theConfiguration);
	void gatherCourtierNames();
	void determineHeirs();
	void createReformedFeatures();
	void resolvePrimogeniture(const std::string& genderLaw, const std::pair<int, std::shared_ptr<Character>>& holder) const;
	void resolveUltimogeniture(const std::string& genderLaw, const std::pair<int, std::shared_ptr<Character>>& holder) const;
	void resolveTanistry(const std::string& genderLaw, const std::pair<int, std::shared_ptr<Character>>& holder) const;
	void resolveTurkish(const std::pair<int, std::shared_ptr<Character>>& holder) const;
	void linkCelestialEmperor() const;
	void linkElectors();
	void loadDynasties(const Configuration& theConfiguration);
	void loadProvinces(const Configuration& theConfiguration);

	bool invasion = false;	
	bool wereNoReformations = true;
	bool greekReformation = false;
	date endDate = date("1444.11.11");
	date startDate = date("1.1.1");
	GameVersion CK2Version;
	std::optional<std::pair<std::string, std::shared_ptr<Title>>> hreTitle; // loaded by configuration option.

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
	Flags flags;
	Vars vars;
	Religions religions;
	mappers::ShatterEmpiresMapper shatterEmpiresMapper;
	mappers::IAmHreMapper iAmHreMapper;
	mappers::PersonalityScraper personalityScraper;
	mappers::ProvinceTitleMapper provinceTitleMapper;
	mappers::ReformedReligionMapper reformedReligionMapper;
	std::map<std::string, std::shared_ptr<Title>> independentTitles;
	std::map<std::string, Liege> dynamicTitles; // Reusing Liege as it has identical structure
	std::vector<mappers::ReformedReligionMapping> religionReforms;
	std::vector<mappers::ReformedReligionMapping> unreligionReforms;
	std::set<std::string> reformationList;
	std::set<std::string> unreformationList;
	std::set<std::string> existentPremadeMonuments;
};
} // namespace CK2

#endif // CK2_WORLD_H
