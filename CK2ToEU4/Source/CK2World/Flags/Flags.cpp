#include "Flags.h"
#include "ParserHelpers.h"
#include "Log.h"


CK2::Flags::Flags(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void CK2::Flags::registerKeys()
{
	registerRegex(commonItems::catchallRegex, [this](const std::string& flagname, std::istream& theStream) {
		flags.insert(flagname);
		commonItems::ignoreItem(flagname, theStream);
	});
}

std::set<std::string> CK2::Flags::fillReformationList()
{
	std::set<std::string> reformationList;
	// Pagan Reformation?
	if (aztecReformation())
		reformationList.insert("aztec_reformed");
	if (balticReformation())
		reformationList.insert("baltic_pagan_reformed");
	if (bonReformation())
		reformationList.insert("bon_reformed");
	if (finnishReformation())
		reformationList.insert("finnish_pagan_reformed");
	if (hellenicReformation())
	{
		reformationList.insert("hellenic_pagan_reformed");
		if (wasGreek())
			greekReformation = true;
	}
	if (norseReformation())
		reformationList.insert("norse_pagan_reformed");
	if (slavicReformation())
		reformationList.insert("slavic_pagan_reformed");
	if (tengriReformation())
		reformationList.insert("tengri_pagan_reformed");
	if (africanReformation())
		reformationList.insert("west_african_pagan_reformed");
	if (zunReformation())
		reformationList.insert("zun_pagan_reformed");

	return reformationList;
}
