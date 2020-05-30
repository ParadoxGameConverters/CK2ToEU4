#include "Character.h"
#include "../Dynasties/Dynasty.h"
#include "Domain.h"
#include "Log.h"
#include "ParserHelpers.h"

CK2::Character::Character(std::istream& theStream, int chrID): charID(chrID)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void CK2::Character::registerKeys()
{
	registerRegex("bn|name", [this](const std::string& unused, std::istream& theStream) {
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
	registerKeyword("fem", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString femStr(theStream);
		female = femStr.getString() == "yes";
	});
	registerKeyword("gov", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString govStr(theStream);
		government = govStr.getString();
	});
	registerKeyword("job", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString jobStr(theStream);
		job = jobStr.getString();
	});
	registerKeyword("md", [this](const std::string& unused, std::istream& theStream) {
		const auto modifierString = commonItems::singleItem(unused, theStream);
		// We have no interest in parsing modifiers. We're looking for one explicit modifier.
		loan = modifierString.find("borrowed_from_jews") != std::string::npos;
	});
	registerKeyword("tr", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::intList trList(theStream);
		for (const auto trait: trList.getInts())
			traits.insert(std::pair(trait, std::string()));
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
		dynasty = std::pair(dynastyInt.getInt(), nullptr);
	});
	registerKeyword("lge", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleInt liegeInt(theStream);
		liege = std::pair(liegeInt.getInt(), nullptr);
	});
	registerKeyword("mot", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleInt motInt(theStream);
		mother = std::pair(motInt.getInt(), nullptr);
	});
	registerKeyword("fat", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleInt fatInt(theStream);
		father = std::pair(fatInt.getInt(), nullptr);
	});
	registerKeyword("piety", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleDouble pieryDbl(theStream);
		piety = pieryDbl.getDouble();
	});
	registerKeyword("wealth", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleDouble wealthDbl(theStream);
		wealth = wealthDbl.getDouble();
	});
	registerKeyword("prs", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleDouble prsDbl(theStream);
		prestige = prsDbl.getDouble();
	});
	registerKeyword("host", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleInt hostInt(theStream);
		host = hostInt.getInt();
	});
	registerKeyword("att", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::intList skillsList(theStream);
		const auto theList = skillsList.getInts();
		skills.diplomacy = theList[0];
		skills.martial = theList[1];
		skills.stewardship = theList[2];
		skills.intrigue = theList[3];
		skills.learning = theList[4];
	});
	registerKeyword("spouse", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleInt spouseInt(theStream);
		spouses.insert(std::pair(spouseInt.getInt(), nullptr));
	});
	registerKeyword("dmn", [this](const std::string& unused, std::istream& theStream) {
		const auto newDomain = Domain(theStream);
		primaryTitle = newDomain.getPrimaryTitle();
		capital = newDomain.getCapital();
	});
	registerRegex("[A-Za-z0-9\\:_.-]+", commonItems::ignoreItem);
}

bool CK2::Character::hasTrait(const std::string& wantedTrait) const
{
	for (const auto& trait: traits)
		if (trait.second == wantedTrait)
			return true;
	return false;
}

const std::string& CK2::Character::getReligion() const
{
	// The CK2 save omits the character religion in the case where the character religion matches the dynasty religion.
	if (religion.empty() && dynasty.second)
	{
		const std::string& dynastyReligion = dynasty.second->getReligion();
		if (!dynastyReligion.empty())
			return dynastyReligion;
	}

	return religion;
}

const std::string& CK2::Character::getCulture() const
{
	if (!culture.empty())
		return culture;
	if (dynasty.second && !dynasty.second->getCulture().empty())
		return dynasty.second->getCulture();
	return culture;
}
