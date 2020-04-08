#ifndef GOVERNMENTS_MAPPING
#define GOVERNMENTS_MAPPING

#include "newParser.h"
#include <set>

namespace mappers
{
class GovernmentsMapping: commonItems::parser
{
  public:
	GovernmentsMapping() = default;
	explicit GovernmentsMapping(std::istream& theStream);

	[[nodiscard]] const auto& getGovernment() const { return government; }
	[[nodiscard]] const auto& getCK2Governments() const { return ck2governments; }
	[[nodiscard]] const auto& getReform() const { return reform; }
	[[nodiscard]] const auto& getCK2Title() const { return ck2title; }

	[[nodiscard]] bool matchGovernment(const std::string& ck2Government, const std::string& incTitle) const;

  private:
	void registerKeys();

	std::string government;
	std::set<std::string> ck2governments;
	std::string reform;
	std::string ck2title;
};
} // namespace mappers

#endif // GOVERNMENTS_MAPPING