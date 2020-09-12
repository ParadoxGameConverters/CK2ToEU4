#ifndef CK2_FLAGS_H
#define CK2_FLAGS_H
#include "Parser.h"
#include <set>

namespace CK2
{
	class Flags;
	class Flags: commonItems::parser
	{
	  public:
		Flags() = default;
		explicit Flags(std::string unused, std::istream& theStream);

		[[nodiscard]] const auto& getFlags() const { return flags; }
		std::set<std::string> checkReformation();
		bool getInvasion();
		bool isGreek() { return greekReformation; }

	  private:
		void registerKeys(std::string unused, std::istream& theStream);

		bool greekReformation = false;
		bool sunsetInvasion = false;
		std::string flags;
	};

} // namespace CK2

#endif // CK2_FLAGS_H