#ifndef MONUMENTS_MAPPER
#define MONUMENTS_MAPPER
#include "MonumentsMapping.h"
#include "Parser.h"

namespace mappers
{
class MonumentsMapper: commonItems::parser
{
  public:
	MonumentsMapper();
	MonumentsMapper(std::istream& theStream);

	[[nodiscard]] const auto& getWonders() const { return wonders; }

  private:
	void registerKeys();

	std::map<std::string, MonumentsMapping> wonders;
};
} // namespace mappers

#endif // CK2_MONUMENTS_MAPPER_H