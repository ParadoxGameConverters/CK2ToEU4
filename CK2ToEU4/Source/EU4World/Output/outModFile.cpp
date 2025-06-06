#include "outModFile.h"

std::ostream& EU4::operator<<(std::ostream& output, const ModFile& modFile)
{
	output << "name = \"Converted - " << modFile.outname.string() << "\"\n";
	output << "path = \"mod/" << modFile.outname.string() << "\"\n";
	output << "normal_or_historical_nations = yes\n";
	output << "supported_version = \"v" << modFile.version.toWildCard() << "\"\n";
	output << "replace_path = \"history/countries/\"\n";
	output << "replace_path = \"history/provinces/\"\n";
	output << "replace_path = \"history/diplomacy/\"\n";
	output << "replace_path = \"history/wars/\"\n";
	output << "replace_path = \"common/countries/\"\n";
	output << "replace_path = \"common/country_tags/\"\n";
	output << "replace_path = \"common/bookmarks/\"\n";
	output << "replace_path = \"common/technologies/\"\n";
	return output;
}
