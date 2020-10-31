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

void CK2::Dynasty::underUpdateDynasty(std::istream& theStream)
{
	registerUnderKeys();
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
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}

void CK2::Dynasty::registerUnderKeys()
{
	registerKeyword("name", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString nameStr(theStream);
		if (name.empty())
			name = nameStr.getString();
	});
	registerKeyword("culture", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString cultureStr(theStream);
		if (culture.empty())
			culture = cultureStr.getString();
	});
	registerKeyword("religion", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString religionStr(theStream);
		if (religion.empty())
			religion = religionStr.getString();
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}

const std::string& CK2::Dynasty::getReligion() const
{
	// CK2 seems to prefer the dynasty religion in the coat_of_arms section. As such we only
	// fall back to the nominal dynasty religion if there's no religion in the coat_of_arms section.
	if (!coa.getReligion().empty())
		return coa.getReligion();
	return religion;
}
