#include "Province.h"
#include "ParserHelpers.h"
#include "Log.h"

CK2::Province::Province(std::istream& theStream, int provID) : provinceID(provID)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void CK2::Province::registerKeys()
{
	registerKeyword("name", [this](const std::string& unused, std::istream& theStream) {
			const commonItems::singleString nameStr(theStream);
			name = nameStr.getString();
		});
	registerKeyword("culture", [this](const std::string& unused, std::istream& theStream) {
			const commonItems::singleString cultureStr(theStream);
			culture = cultureStr.getString();
		});
	registerKeyword("religion", [this](const std::string& unused, std::istream& theStream) {
			const commonItems::singleString religionStr(theStream);
			religion = religionStr.getString();
		});
	registerKeyword("primary_settlement", [this](const std::string& unused, std::istream& theStream) {
			const commonItems::singleString primarySettlementStr(theStream);
			primarySettlement = primarySettlementStr.getString();
		});
	registerRegex("[A-Za-z0-9\\:_.-]+", commonItems::ignoreItem);
}

