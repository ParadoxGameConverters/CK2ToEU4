#ifndef CK2_RELATIONS_H
#define CK2_RELATIONS_H
#include "Relation.h"
#include "newParser.h"

namespace CK2
{
class Relations: commonItems::parser
{
  public:
	Relations() = default;
	Relations(std::istream& theStream, std::string first);

	[[nodiscard]] const auto& getRelations() const { return relations; }

  private:
	void registerKeys(int first);
	std::vector<Relation> relations;
};
} // namespace CK2

#endif // CK2_RELATIONS_H
