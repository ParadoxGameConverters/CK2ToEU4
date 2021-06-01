#include "Relations.h"
#include "ParserHelpers.h"
#include "CommonRegexes.h"

CK2::Relations::Relations(std::istream& theStream, std::string first)
{
	const auto pos = first.find('_');
	const auto firstID = std::stoi(first.substr(pos + 1, first.length()));
	registerKeys(firstID);
	parseStream(theStream);
	clearRegisteredKeywords();
}

void CK2::Relations::registerKeys(int first)
{
	registerRegex("\\d+", [this, first](const std::string& second, std::istream& theStream) {
		auto newRelation = Relation(theStream, std::stoi(second));
		newRelation.setFirst(first);
		relations.emplace_back(newRelation);
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}
