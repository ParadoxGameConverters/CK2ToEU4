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

		std::optional<int> getIDforTitle(const std::string& title);
		std::optional<std::string> getTitleforID(int provID);

	private:
		
		std::map<std::string, int> provinceTitles; // beware, c_titles can have identical IDs, thanx paradox.
	};
}

#endif // PROVINCE_TITLE_MAPPER