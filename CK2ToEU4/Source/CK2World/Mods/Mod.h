#ifndef CK2_MOD_H
#define CK2_MOD_H

#include "newParser.h"

namespace CK2
{
class Mod: commonItems::parser
{
  public:
	explicit Mod(std::istream& theStream);

	[[nodiscard]] const auto& getName() const { return name; }
	[[nodiscard]] const auto& getPath() const { return path; }
	[[nodiscard]] auto looksValid() const { return !name.empty() && !path.empty(); }
	[[nodiscard]] auto isCompressed() const { return compressed; }

  private:
	std::string name;
	std::string path;
	bool compressed = false;
};
} // namespace CK2

#endif // CK2_MOD_H