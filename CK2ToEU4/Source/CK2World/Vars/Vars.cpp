#include "Vars.h"
#include "ParserHelpers.h"

CK2::Vars::Vars(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void CK2::Vars::registerKeys()
{
	registerRegex("[A-Za-z0-9_.-]+", [this](const std::string& varName, std::istream& theStream) {
		const commonItems::singleDouble valueDbl(theStream);
		vars.insert(std::pair(varName, valueDbl.getDouble()));
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}

std::optional<std::map<std::string, double>> CK2::Vars::getChineseReligions() const
{
	std::map<std::string, double> toReturn;
	for (const auto& var: vars)
	{
		if (var.first.find("global_chinese_") != 0)
			continue;
		if (var.first.length() < 17)
			continue;
		const auto subVar = var.first.substr(15, var.first.length() - 16);
		toReturn.insert(std::pair(subVar, var.second));
	}
	if (!toReturn.empty())
		return toReturn;
	return std::nullopt;
}