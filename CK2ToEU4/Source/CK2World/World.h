#ifndef CK2_WORLD_H
#define CK2_WORLD_H
#include "newParser.h"

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
				
		struct saveData
		{
			bool compressed = false;
			std::string metadata;
			std::string gamestate;
		};
		saveData saveGame;
		
		//std::unique_ptr<Version> version;
		
	};
}

#endif // CK2_WORLD_H
