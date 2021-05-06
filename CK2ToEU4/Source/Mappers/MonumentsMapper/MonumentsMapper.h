#ifndef MONUMENTS_MAPPER
#define MONUMENTS_MAPPER
#include "Parser.h"
#include "MonumentsMapping.h"

namespace mappers
{
class MonumentsMapper: commonItems::parser
{
  public:
	MonumentsMapper() = default;
	MonumentsMapper(std::istream& theStream);

	std::map<int, std::string> getWonders() { return wonders; }

  private:
	void registerKeys();

	std::map<int, MonumentsMapping> wonders;
};
} // namespace mappers

#endif // CK2_MONUMENTS_MAPPER_H