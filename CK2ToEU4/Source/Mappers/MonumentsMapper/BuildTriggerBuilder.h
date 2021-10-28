#ifndef BUILD_TRIGGER_BUILDER
#define BUILD_TRIGGER_BUILDER
#include "Parser.h"

namespace mappers
{
class BuildTriggerBuilder: commonItems::parser
{
  public:
	BuildTriggerBuilder() = default;
	explicit BuildTriggerBuilder(std::istream& theStream);

	[[nodiscard]] const auto& getBuildTrigger() const { return buildTrigger; }
	[[nodiscard]] auto getCultural() const { return cultural; }
	[[nodiscard]] auto getReligious() const { return religious; }

  private:
	void registerKeys();

	bool cultural = false;
	bool religious = false;
	std::string buildTrigger;
};
} // namespace mappers

#endif // BUILD_TRIGGER_BUILDER_H