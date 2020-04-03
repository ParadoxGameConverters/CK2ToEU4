#ifndef CK2_DOMAIN_H
#define CK2_DOMAIN_H
#include "newParser.h"

namespace CK2
{
	class Barony;
	class Title;
	class Domain: commonItems::parser
	{
	public:
		Domain() = default;
		Domain(std::istream& theStream);

		[[nodiscard]] const auto& getCapital() const { return capital; }
		[[nodiscard]] const auto& getPrimaryTitle() const { return primaryTitle; }

	private:
		void registerKeys();

		std::pair<std::string, std::shared_ptr<Barony>> capital;
		std::pair<std::string, std::shared_ptr<Title>> primaryTitle;
	};
}

#endif // CK2_DOMAIN_H
