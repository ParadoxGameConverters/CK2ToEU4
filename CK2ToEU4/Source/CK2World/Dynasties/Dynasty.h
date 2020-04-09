#ifndef CK2_DYNASTY_H
#define CK2_DYNASTY_H
#include "newParser.h"

namespace CK2
{
class Dynasty: commonItems::parser
{
  public:
	Dynasty(std::istream& theStream, int theDynID);

	void updateDynasty(std::istream& theStream);

	[[nodiscard]] const auto& getCulture() const { return culture; }
	[[nodiscard]] const auto& getReligion() const { return religion; }
	[[nodiscard]] const auto& getName() const { return name; }

	[[nodiscard]] auto getID() const { return dynID; }	

  private:
	void registerKeys();

	int dynID = 0;
	std::string culture;
	std::string religion;
	std::string name;
};
} // namespace CK2

#endif // CK2_DYNASTY_H
