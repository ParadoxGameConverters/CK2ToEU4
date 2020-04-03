#ifndef PROVINCE_TITLE_MAPPER
#define PROVINCE_TITLE_MAPPER

#include "newParser.h"

namespace mappers
{
	class ProvinceTitleMapper
	{
	public:
		ProvinceTitleMapper() = default;
		void loadProvinces(const std::string& CK2Path);

		[[nodiscard]] const auto& getProvinceTitles() const { return provinceTitles; }
		void replaceProvinceTitles(std::map<std::string, int> newProvinceTitles) { provinceTitles = std::move(newProvinceTitles); }

		std::optional<int> getIDForTitle(const std::string& title);
		std::optional<std::string> getTitleForID(int provID);

	private:
		
		std::map<std::string, int> provinceTitles; // beware, c_titles can have identical IDs (until filtered), thanx paradox.
	};
}

#endif // PROVINCE_TITLE_MAPPER