#ifndef BUILD_TRIGGER_BUILDER
#define BUILD_TRIGGER_BUILDER
#include "Parser.h"


namespace mappers
{
	class BuildTriggerBuilder: commonItems::parser
	{
	  public:
		BuildTriggerBuilder();
		BuildTriggerBuilder(std::istream& theStream);

		[[nodiscard]] auto getBuildTrigger() { return buildTrigger; }
		[[nodiscard]] auto getCultural() { return cultural; }
		[[nodiscard]] auto getReligious() { return religious; }

	  private:
		void registerKeys();

		bool cultural = false;
		bool religious = false;
		std::string buildTrigger;
	};
} // namespace mappers

#endif // BUILD_TRIGGER_BUILDER_H