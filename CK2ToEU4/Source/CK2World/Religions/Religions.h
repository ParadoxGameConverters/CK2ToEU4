#ifndef CK2_RELIGIONS_H
#define CK2_RELIGIONS_H
#include "Parser.h"

namespace CK2
{
class Religion;
class Religions: commonItems::parser
{
  public:
	Religions() = default;
	explicit Religions(std::istream& theStream);

	[[nodiscard]] const auto& getReformedReligion() const { return reformedReligions; }

  private:
	void registerKeys();

	std::map<std::string, std::vector<std::string>> reformedReligions;
};

} // namespace CK2

#endif // CK2_RELIGIONS_H
