#include "AllRelations.h"
#include "CommonRegexes.h"
#include "ParserHelpers.h"

CK2::Diplomacy::Diplomacy(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void CK2::Diplomacy::registerKeys()
{
	registerRegex(R"(diplo_\d+)", [this](const std::string& first, std::istream& theStream) {
		auto newRelations = Relations(theStream, first);
		const auto pos = first.find('_');
		const auto firstID = std::stoi(first.substr(pos + 1, first.length()));
		diplomacy.insert(std::pair(firstID, newRelations));
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}
