#ifndef PROVINCE_TITLE_GRABBER
#define PROVINCE_TITLE_GRABBER

#include "Parser.h"

namespace mappers
{
class ProvinceTitleGrabber: commonItems::parser
{
  public:
	explicit ProvinceTitleGrabber(const std::string& provincePath);

	[[nodiscard]] auto getID() const { return provID; }
	[[nodiscard]] const auto& getTitle() const { return title; }

  private:
	void registerKeys();

	int provID = 0;
	std::string title;
};
} // namespace mappers

#endif // PROVINCE_TITLE_GRABBER