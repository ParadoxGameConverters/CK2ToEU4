#ifndef CK2_WORLD_H
#define CK2_WORLD_H
#include "newParser.h"
#include "Date.h"
#include "../Mappers/ProvinceTitleMapper/ProvinceTitleMapper.h"
#include "Provinces/Provinces.h"
#include "Characters/Characters.h"
#include "Titles/Titles.h"
#include "Dynasties/Dynasties.h"
#include "../Common/Version.h"
#include "Titles/Liege.h"

class Configuration;

namespace CK2
{
	class World: commonItems::parser
	{
	public:
		World(std::shared_ptr<Configuration> theConfiguration);
				
	private:
		void verifySave(const std::string& saveGamePath);
		bool uncompressSave(const std::string& saveGamePath);
		void filterIndependentTitles();
		void mergeIndependentBaronies() const;
		void congregateProvinces();
		void mergeRevolts();
		void sanityCheckifyProvinces();
		
		date endDate = date("1444.11.11");
		date startDate = date("1.1.1");
		Version CK2Version;

		struct saveData
		{
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
		std::map<std::string, Liege> dynamicTitles; // Reusing Liege as it has identical structure

		std::map<std::string, std::shared_ptr<Title>> independentTitles;
	};
}

#endif // CK2_WORLD_H
