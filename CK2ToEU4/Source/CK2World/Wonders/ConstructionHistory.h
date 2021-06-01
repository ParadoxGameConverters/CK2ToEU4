#ifndef CK2_CONSTRUCTIONHISTORY_H
#define CK2_CONSTRUCTIONHISTORY_H
#include "Parser.h"

namespace CK2
{
class ConstructionHistory: commonItems::parser
{
  public:
	ConstructionHistory() = default;
	explicit ConstructionHistory(std::istream& theStream);

	[[nodiscard]] auto getBuilderID() const { return builderID; }
	[[nodiscard]] auto getBinaryDate() const { return binaryDate; }
	[[nodiscard]] const auto& getUpgrade() const { return upgrade; }

  private:
	void registerKeys();

	int builderID = 0;
	int binaryDate = 0;
	std::string upgrade;
};
} // namespace CK2

#endif // CK2_CONSTRUCTIONHISTORY_H
