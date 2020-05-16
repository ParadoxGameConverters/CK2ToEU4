#ifndef PROVINCE_TITLE_MAPPER
#define PROVINCE_TITLE_MAPPER

#include "Parser.h"

namespace CK2
{
class Provinces;
class Titles;
} // namespace CK2

namespace mappers
{
class ProvinceTitleMapper
{
  public:
	ProvinceTitleMapper() = default;
	void loadProvinces(const std::string& CK2Path);

	[[nodiscard]] const auto& getProvinceTitles() const { return provinceTitles; }
	[[nodiscard]] const auto& getOrigProvinceTitles() const { return origProvinceTitles; }
	void filterSelf(const CK2::Provinces& theProvinces, const CK2::Titles& theTitles);

	[[nodiscard]] std::optional<int> getIDForTitle(const std::string& title) const;
	[[nodiscard]] std::optional<std::string> getTitleForID(int provID) const;

  private:
	std::multimap<std::string, int> origProvinceTitles; // c_title can have multiple IDs, and IDs can have multiple c_titles, thanx paradox.
	std::map<std::string, int> provinceTitles;			 // filtered list.
};
} // namespace mappers

#endif // PROVINCE_TITLE_MAPPER