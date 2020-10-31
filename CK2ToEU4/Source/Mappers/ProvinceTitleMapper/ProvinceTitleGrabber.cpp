#include "ProvinceTitleGrabber.h"
#include "CommonFunctions.h"
#include "OSCompatibilityLayer.h"
#include "ParserHelpers.h"

mappers::ProvinceTitleGrabber::ProvinceTitleGrabber(const std::string& provincePath)
{
	if (!commonItems::DoesFileExist(provincePath))
		throw std::runtime_error(provincePath + " does not exist?");
	registerKeys();
	parseFile(provincePath);
	clearRegisteredKeywords();

	const auto path = trimPath(provincePath);
	try
	{
		provID = std::stoi(path);
	}
	catch (std::exception& e)
	{
		Log(LogLevel::Warning) << "Province filename " << provincePath << " is not a valid name, skipping this province: " << e.what();
	}
}

void mappers::ProvinceTitleGrabber::registerKeys()
{
	registerKeyword("title", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString titleStr(theStream);
		title = titleStr.getString();
	});

	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}