#include "Character.h"
#include "ParserHelpers.h"
#include "Log.h"

CK2::Character::Character(std::istream& theStream, int chrID) : charID(chrID)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void CK2::Character::registerKeys()
{
	registerKeyword("bn", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString nameStr(theStream);
		name = nameStr.getString();
		});
	registerKeyword("cul", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString cultureStr(theStream);
		culture = cultureStr.getString();
		});
	registerKeyword("rel", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString religionStr(theStream);
		religion = religionStr.getString();
		});
	registerKeyword("b_d", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString dateStr(theStream);
		birthDate = date(dateStr.getString());
		});
	registerKeyword("d_d", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString dateStr(theStream);
		deathDate = date(dateStr.getString());
		});
	registerKeyword("dnt", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleInt dynastyInt(theStream);
		dynasty = dynastyInt.getInt();
		});
	registerRegex("[A-Za-z0-9\\:_.-]+", commonItems::ignoreItem);
}
