#ifndef CK2_WONDERS_H
#define CK2_WONDERS_H
#include "Parser.h"

namespace CK2
{
class Wonder;
class Wonders: commonItems::parser
{
  public:
	Wonders() = default;
	explicit Wonders(std::istream& theStream);

	[[nodiscard]] const auto& getWonders() const { return wonders; }

  private:
	void registerKeys();

	std::map<int, std::shared_ptr<Wonder>> wonders;
};
} // namespace CK2

#endif // CK2_WONDERS_H
