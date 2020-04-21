#include "Offmaps.h"
#include "Log.h"
#include "Offmap.h"
#include "ParserHelpers.h"

CK2::Offmaps::Offmaps(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void CK2::Offmaps::registerKeys()
{
	registerRegex(R"(\d+)", [this](const std::string& wonderID, std::istream& theStream) {
		auto newOffmap = std::make_shared<Offmap>(theStream);
		offmaps.insert(std::pair(std::stoi(wonderID), newOffmap));
	});
	registerRegex("[A-Za-z0-9\\_:.-]+", commonItems::ignoreItem);
}

std::optional<std::pair<int, std::shared_ptr<CK2::Offmap>>> CK2::Offmaps::getChina() const
{
	for (const auto& offmap: offmaps)
		if (offmap.second->getType() == "offmap_china")
			return offmap;
	return std::nullopt;
}
