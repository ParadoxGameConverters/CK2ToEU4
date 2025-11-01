#ifndef CK2_LIEGE_H
#define CK2_LIEGE_H
#include "Parser.h"

namespace CK2
{
class Title;
class Liege: commonItems::parser
{
  public:
	Liege() = default;
	explicit Liege(std::istream& theStream);
	explicit Liege(const std::string& theTitle): title(std::make_pair(theTitle, nullptr)) {};

	void setTitle(std::shared_ptr<Title> theTitle) { title.second = std::move(theTitle); }
	void setBaseTitle(std::shared_ptr<Title> theBaseTitle) { baseTitle.second = std::move(theBaseTitle); }

	[[nodiscard]] const auto& getTitle() const { return title; }
	[[nodiscard]] const auto& getBaseTitle() const { return baseTitle; }
	[[nodiscard]] auto isDynamic() const { return dynamic; }
	[[nodiscard]] auto isCustom() const { return custom; }

  private:
	void registerKeys();

	bool dynamic = false;
	bool custom = false;
	std::pair<std::string, std::shared_ptr<Title>> title;
	std::pair<std::string, std::shared_ptr<Title>> baseTitle;
};
} // namespace CK2

#endif // CK2_LIEGE_H
