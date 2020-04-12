#ifndef CK2_TITLES_H
#define CK2_TITLES_H
#include "newParser.h"

namespace mappers
{
class ProvinceTitleMapper;
}

namespace CK2
{
class Title;
class Characters;
class Provinces;
class Titles: commonItems::parser
{
  public:
	Titles() = default;
	Titles(std::istream& theStream);
	[[nodiscard]] const auto& getTitles() const { return titles; }

	void linkHolders(const Characters& theCharacters);
	void linkPreviousHolders(const Characters& theCharacters);
	void linkLiegePrimaryTitles();
	void linkVassals();
	void linkBaseTitles();
	void linkProvinces(const Provinces& theProvinces, const mappers::ProvinceTitleMapper& provinceTitleMapper);
	void mergeRevolts();

  private:
	void registerKeys();

	std::map<std::string, std::shared_ptr<Title>> titles;
};
} // namespace CK2

#endif // CK2_TITLES_H
