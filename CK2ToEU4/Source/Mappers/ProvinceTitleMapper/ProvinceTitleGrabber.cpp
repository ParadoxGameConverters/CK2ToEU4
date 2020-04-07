#include "ProvinceTitleGrabber.h"
#include "../../Common/CommonFunctions.h"
#include "OSCompatibilityLayer.h"
#include "ParserHelpers.h"

mappers::ProvinceTitleGrabber::ProvinceTitleGrabber(const std::string& provincePath)
{
	if (!Utils::DoesFileExist(provincePath)) throw std::runtime_error(provincePath + " does not exist?");
	registerKeys();
	parseFile(provincePath);
	clearRegisteredKeywords();

	const auto path = trimPath(provincePath);
	const auto minusPosition = path.find(" - ");
	const auto idString = path.substr(0, minusPosition);
	provID = std::stoi(idString);
}

void mappers::ProvinceTitleGrabber::registerKeys()
{
	registerKeyword("title", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString titleStr(theStream);
		title = titleStr.getString();
	});

	registerRegex("[A-Za-z0-9\\_.:-]+", commonItems::ignoreItem);
}