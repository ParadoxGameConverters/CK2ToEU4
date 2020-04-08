#include "outModFile.h"

std::ostream& EU4::operator<<(std::ostream& output, const ModFile& modFile)
{
	output << "name = \"Converted - " << modFile.outname << "\"\n";
	output << "path = \"mod/" << modFile.outname << "\"\n";
	output << "user_dir = \"" << modFile.outname << "\"\n";
	output << "normal_or_historical_nations = yes\n";
	output << "supported_version = \"1.29.*\"\n";
	output << "replace_path = \"history/countries\"\n";
	output << "replace_path = \"history/provinces\"\n";
	//	output << "replace = \"history/diplomacy\"\n";
	//	output << "replace = \"history/units\"\n";
//	output << "replace = \"history/wars\"\n";
	output << "replace_path = \"common/countries\"\n";
	output << "replace_path = \"common/country_tags\"\n";
//	output << "replace = \"localisation/0_Names.csv\"\n";
//	output << "replace = \"localisation/0_Cultures.csv\"\n";
//	output << "replace = \"localisation/0_Religions.csv\"\n";
	return output;
}
