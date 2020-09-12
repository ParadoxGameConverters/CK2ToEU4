#include "Flags.h"
#include "ParserHelpers.h"
#include "Log.h"


CK2::Flags::Flags(std::string unused, std::istream& theStream)
{
	registerKeys(unused, theStream);
	parseStream(theStream);
	clearRegisteredKeywords();
}

void CK2::Flags::registerKeys(std::string unused, std::istream& theStream)
{
	flags = commonItems::singleItem(unused, theStream);
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}

std::set<std::string> CK2::Flags::checkReformation()
{
	std::set<std::string> reformationList;
	// Pagan Reformation?
	if (flags.find("aztec_reformation") != std::string::npos)
		reformationList.insert("aztec_reformed");
	if (flags.find("baltic_reformation") != std::string::npos)
		reformationList.insert("baltic_pagan_reformed");
	if (flags.find("bon_reformation") != std::string::npos)
		reformationList.insert("bon_reformed");
	if (flags.find("finnish_reformation") != std::string::npos)
		reformationList.insert("finnish_pagan_reformed");
	if (flags.find("hellenic_reformation") != std::string::npos)
	{
		reformationList.insert("hellenic_pagan_reformed");
		if (flags.find("flag_hellenic_greek_reformation") != std::string::npos)
			greekReformation = true;
	}
	if (flags.find("norse_reformation") != std::string::npos)
		reformationList.insert("norse_pagan_reformed");
	if (flags.find("slavic_reformation") != std::string::npos)
		reformationList.insert("slavic_pagan_reformed");
	if (flags.find("tengri_reformation") != std::string::npos)
		reformationList.insert("tengri_pagan_reformed");
	if (flags.find("west_african_reformation") != std::string::npos)
		reformationList.insert("west_african_pagan_reformed");
	if (flags.find("zun_reformation") != std::string::npos)
		reformationList.insert("zun_pagan_reformed");

	return reformationList;
}

bool CK2::Flags::getInvasion()
{
	if (flags.find("aztec_explorers") != std::string::npos)
	{
		sunsetInvasion = true;
		LOG(LogLevel::Info) << "oO Invasion detected. We're in for a ride!";
	}		
	return sunsetInvasion;
}
