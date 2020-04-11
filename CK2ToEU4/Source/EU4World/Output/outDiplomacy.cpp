#include "../Diplomacy/Diplomacy.h"
#include "Log.h"
#include <fstream>
#include "../../Configuration/Configuration.h"

void EU4::Diplomacy::output(const Configuration& theConfiguration) const
{
	std::ofstream alliances("output/" + theConfiguration.getOutputName() + "/history/diplomacy/converter_alliances.txt");
	if (!alliances.is_open()) throw std::runtime_error("Could not create alliances history file!");

	std::ofstream guarantees("output/" + theConfiguration.getOutputName() + "/history/diplomacy/converter_guarantees.txt");
	if (!guarantees.is_open()) throw std::runtime_error("Could not create guarantees history file!");

	std::ofstream puppetStates("output/" + theConfiguration.getOutputName() + "/history/diplomacy/converter_puppetstates.txt");
	if (!puppetStates.is_open()) throw std::runtime_error("Could not create puppet states history file!");

	std::ofstream unions("output/" + theConfiguration.getOutputName() + "/history/diplomacy/converter_unions.txt");
	if (!unions.is_open()) throw std::runtime_error("Could not create unions history file!");
	
	for (const auto& agreement: agreements)
	{
		if (agreement.getType() == "guarantee")
		{
			guarantees << agreement;
		}
		else if (agreement.getType() == "union")
		{
			unions << agreement;
		}
		else if (agreement.getType() == "vassal")
		{
			puppetStates << agreement;
		}
		else if (agreement.getType() == "alliance")
		{
			alliances << agreement;
		}
		else
		{
			LOG(LogLevel::Warning) << "Cannot output diplomatic agreement type " << agreement.getType() << "!";
		}
	}
	
	alliances.close();
	guarantees.close();
	puppetStates.close();
	unions.close();
}