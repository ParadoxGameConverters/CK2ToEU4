#ifndef CK2_LIEGE_H
#define CK2_LIEGE_H
#include "newParser.h"

namespace CK2
{
	class Title;
	class Liege: commonItems::parser
	{
	public:
		Liege() = default;
		Liege(std::istream& theStream);
		Liege(const std::string& theTitle) : title(std::make_pair(theTitle, nullptr)){};
		
		[[nodiscard]] const auto& getTitle() const { return title; }
		[[nodiscard]] const auto& getBaseTitle() const { return baseTitle; }
		[[nodiscard]] auto isDynamic() const { return dynamic; }
		[[nodiscard]] auto isCustom() const { return custom; }

	private:
		void registerKeys();

		std::pair<std::string, std::shared_ptr<Title>> title;
		std::pair<std::string, std::shared_ptr<Title>> baseTitle;
		bool dynamic = false;
		bool custom = false;		
	};
}

#endif // CK2_LIEGE_H
