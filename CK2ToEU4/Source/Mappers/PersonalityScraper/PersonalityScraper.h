#ifndef PERSONALITY_SCRAPER
#define PERSONALITY_SCRAPER

#include "newParser.h"

class Configuration;
namespace mappers
{
class PersonalityScraper: commonItems::parser
{
  public:
	PersonalityScraper() = default;
	void scrapePersonalities(std::istream& theStream);
	void scrapePersonalities(const Configuration& theConfiguration);

	[[nodiscard]] const auto& getPersonalities() const { return personalities; }

	[[nodiscard]] std::optional<std::string> getPersonalityForID(int ID) const;

  private:
	void registerKeys();

	std::map<int, std::string> personalities;
	int counter = 1; // Yes, 1.
};
} // namespace mappers

#endif // PERSONALITY_SCRAPER