#ifndef CK2_VARS_H
#define CK2_VARS_H
#include "newParser.h"

namespace CK2
{
class Vars: commonItems::parser
{
  public:
	Vars() = default;
	explicit Vars(std::istream& theStream);

	[[nodiscard]] const auto& getVars() const { return vars; } // for testing
	[[nodiscard]] std::optional<std::map<std::string, double>> getChineseReligions() const;
	[[nodiscard]] bool isTengriReformed() const;

  private:
	void registerKeys();

	std::map<std::string, double> vars;
};
} // namespace CK2

#endif // CK2_VARS_H
