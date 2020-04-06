#ifndef I_AM_HRE_MAPPER
#define I_AM_HRE_MAPPER

#include "newParser.h"

namespace mappers
{
	class IAmHreMapper: commonItems::parser
	{
	public:
		IAmHreMapper();
		IAmHreMapper(std::istream& theStream);

		[[nodiscard]] const auto& getHRE() const { return hre; }

	private:
		void registerKeys();

		std::string hre;
	};
}

#endif // I_AM_HRE_MAPPER