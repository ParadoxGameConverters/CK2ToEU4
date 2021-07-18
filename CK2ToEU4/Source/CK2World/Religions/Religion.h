#ifndef CK2_RELIGION_H
#define CK2_RELIGION_H
#include "Parser.h"

namespace CK2
{
class Religion: commonItems::parser
{
  public:
	Religion(std::istream& theStream);
	[[nodiscard]] const auto& getFeatures() const { return features; }

  private:
	void registerKeys();

	std::vector<std::string> features;
};
} // namespace CK2

#endif // CK2_RELIGIONS_H
