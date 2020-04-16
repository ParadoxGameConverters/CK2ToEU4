#include "Dynasty.h"
#include "Log.h"
#include "ParserHelpers.h"

CK2::Dynasty::Dynasty(std::istream& theStream, int theDynID): dynID(theDynID)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void CK2::Dynasty::updateDynasty(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void CK2::Dynasty::registerKeys()
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
	registerKeyword("coat_of_arms", [this](const std::string& unused, std::istream& theStream) {
		coa = CoatOfArms(theStream);
	});
	registerRegex("[A-Za-z0-9\\:_.-]+", commonItems::ignoreItem);
}

const std::string& CK2::Dynasty::getReligion() const
{
	if (!religion.empty()) return religion;
	if (!coa.getReligion().empty()) return coa.getReligion();
	return religion;
}
