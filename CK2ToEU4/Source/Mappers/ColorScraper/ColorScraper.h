#ifndef COLOR_SCRAPER
#define COLOR_SCRAPER

#include "Color.h"
#include "Parser.h"

namespace mappers
{
class ColorScraper: commonItems::parser
{
  public:
	ColorScraper() = default;
	void scrapeColors(std::istream& theStream, std::string theName);
	void scrapeColors(const std::filesystem::path& filePath);

	[[nodiscard]] const auto& getColors() const { return titleColors; }
	[[nodiscard]] std::optional<commonItems::Color> getColorForTitle(const std::string& titleName) const;

  private:
	void registerKeys();

	std::map<std::string, std::optional<commonItems::Color>> titleColors;
	std::string name;
	std::optional<commonItems::Color> color;
};
} // namespace mappers

#endif // COLOR_SCRAPER