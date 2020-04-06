#ifndef TITLE_TAG_MAPPING_H
#define TITLE_TAG_MAPPING_H

#include "newParser.h"
#include <set>

namespace mappers
{
class TitleTagMapping: commonItems::parser
{
  public:
	TitleTagMapping() = default;
	explicit TitleTagMapping(std::istream& theStream);

	[[nodiscard]] std::optional<std::string> titleMatch(const std::string& ck2title) const;
	[[nodiscard]] std::optional<std::string> capitalMatch(int eu4capital) const;

	[[nodiscard]] const auto& getEU4Tag() const { return eu4Tag; } // used for testing
	[[nodiscard]] const auto& getCK2Title() const { return ck2Title; } // used for testing
	[[nodiscard]] const auto& getCapitals() const { return capitals; } // used for testing
	
  private:
	void registerKeys();	

	std::string eu4Tag;
	std::string ck2Title;
	std::set<int> capitals;
};
} // namespace mappers

#endif // TITLE_TAG_MAPPING_H