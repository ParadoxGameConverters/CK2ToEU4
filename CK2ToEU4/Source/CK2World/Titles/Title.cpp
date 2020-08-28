#include "Title.h"
#include "../Characters/Character.h"
#include "../Provinces/Province.h"
#include "Log.h"
#include "ParserHelpers.h"

CK2::Title::Title(std::istream& theStream, std::string theName): name(std::move(theName))
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void CK2::Title::registerKeys()
{
	registerKeyword("holder", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleInt holderInt(theStream);
		holder = std::pair(holderInt.getInt(), nullptr);
	});
	registerKeyword("color", [this](const std::string& unused, std::istream& theStream) {
		color = commonItems::Color::Factory::getColor(theStream);
	});
	registerKeyword("law", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString lawStr(theStream);
		laws.insert(lawStr.getString());
	});
	registerKeyword("name", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString nameStr(theStream);
		displayName = nameStr.getString();
	});
	registerKeyword("previous", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::intList listList(theStream);
		const auto& theList = listList.getInts();
		for (const auto& prevHolder: theList)
			previousHolders.insert(std::pair(prevHolder, nullptr));
	});
	registerKeyword("major_revolt", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString revoltStr(theStream);
		majorRevolt = revoltStr.getString() == "yes";
	});
	registerKeyword("gender", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString genderStr(theStream);
		genderLaw = genderStr.getString();
	});
	registerKeyword("succession", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString successionStr(theStream);
		successionLaw = successionStr.getString();
	});
	registerKeyword("succession_electors", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::intList theList(theStream);
		const auto& electorIDs = theList.getInts();
		electors.insert(electorIDs.begin(), electorIDs.end());
	});
	registerKeyword("base_title", [this](const std::string& unused, std::istream& theStream) {
		// This can either be a single string or a Liege object.
		const auto baseStr = commonItems::singleItem(unused, theStream);
		if (baseStr.find("{") != std::string::npos)
		{
			std::stringstream tempStream(baseStr);
			auto newBaseTitle = std::make_shared<Liege>(tempStream);
			baseTitle = std::pair(newBaseTitle->getTitle().first, newBaseTitle);
		}
		else
		{
			auto newBaseTitle = std::make_shared<Liege>(baseStr);
			baseTitle = std::pair(newBaseTitle->getTitle().first, newBaseTitle);
		}
	});
	registerKeyword("liege", [this](const std::string& unused, std::istream& theStream) {
		// This can either be a single string or a Liege object.
		const auto liegeStr = commonItems::singleItem(unused, theStream);
		if (liegeStr.find("{") != std::string::npos)
		{
			std::stringstream tempStream(liegeStr);
			auto newLiege = std::make_shared<Liege>(tempStream);
			liege = std::pair(newLiege->getTitle().first, newLiege);
		}
		else
		{
			auto newLiege = std::make_shared<Liege>(liegeStr);
			liege = std::pair(newLiege->getTitle().first, newLiege);
		}
	});
	registerKeyword("de_jure_liege", [this](const std::string& unused, std::istream& theStream) {
		// This can again either be a single string or a Liege object.
		const auto djLiegeStr = commonItems::singleItem(unused, theStream);
		if (djLiegeStr.find("{") != std::string::npos)
		{
			std::stringstream tempStream(djLiegeStr);
			auto newdjLiege = std::make_shared<Liege>(tempStream);
			deJureLiege = std::pair(newdjLiege->getTitle().first, newdjLiege);
		}
		else
		{
			auto newdjLiege = std::make_shared<Liege>(djLiegeStr);
			deJureLiege = std::pair(newdjLiege->getTitle().first, newdjLiege);
		}
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}

void CK2::Title::congregateProvinces(const std::map<std::string, std::shared_ptr<Title>>& independentTitles)
{
	// We're gathering vassal provinces and adding to our own, unless they are independent (e_hre and similar).
	for (const auto& vassal: vassals)
	{
		if (!independentTitles.count(vassal.first))
		{
			const auto& vassalProvinces = vassal.second->coalesceProvinces();
			provinces.insert(vassalProvinces.begin(), vassalProvinces.end());
		}
	}
}

void CK2::Title::congregateDeJureProvinces()
{
	// We're gathering de jure vassal de jure provinces and adding to our own.
	for (const auto& deJureVassal: deJureVassals)
	{
		const auto& deJureVassalDeJureProvinces = deJureVassal.second->coalesceDeJureProvinces();
		deJureProvinces.insert(deJureVassalDeJureProvinces.begin(), deJureVassalDeJureProvinces.end());
	}
}

int CK2::Title::flagDeJureHREProvinces()
{
	auto counter = 0;
	// Flag our dejure provinces, as well as our dejure vassals' provinces, wherever those may be.
	// Actual flagging will happen on county level as higher titles have no dejure provinces linked to them at this stage.
	for (const auto& deJureVassal: deJureVassals)
	{
		counter += deJureVassal.second->flagDeJureHREProvinces();
	}
	for (const auto& province: deJureProvinces)
	{
		province.second->setDeJureHRE();
		counter++;
	}
	return counter;
}

std::map<int, std::shared_ptr<CK2::Province>> CK2::Title::coalesceProvinces() const
{
	// We're gathering vassal provinces + our own, and passing them on, adding nothing to ourselves.
	std::map<int, std::shared_ptr<Province>> toReturn;
	for (const auto& vassal: vassals)
	{
		const auto& vassalProvinces = vassal.second->coalesceProvinces();
		toReturn.insert(vassalProvinces.begin(), vassalProvinces.end());
	}
	toReturn.insert(provinces.begin(), provinces.end());
	return toReturn;
}

std::map<int, std::shared_ptr<CK2::Province>> CK2::Title::coalesceDeJureProvinces() const
{
	// We're gathering vassal dejure provinces + our own, and passing them on, adding nothing to ourselves.
	std::map<int, std::shared_ptr<Province>> toReturn;
	for (const auto& deJureVassal: deJureVassals)
	{
		const auto& vassalDeJureProvinces = deJureVassal.second->coalesceDeJureProvinces();
		toReturn.insert(vassalDeJureProvinces.begin(), vassalDeJureProvinces.end());
	}
	toReturn.insert(deJureProvinces.begin(), deJureProvinces.end());
	return toReturn;
}

void CK2::Title::registerGeneratedVassal(const std::pair<std::string, std::shared_ptr<Title>>& theVassal)
{
	generatedVassals.insert(theVassal);
	vassals.erase(theVassal.first);
}
