#ifndef DIPLOMACY_H
#define DIPLOMACY_H

#include "Agreement.h"
#include <map>
#include <vector>
#include "newParser.h"

class Configuration;
namespace EU4
{
class Country;
class Diplomacy: commonItems::parser
{
  public:
	Diplomacy();
	
	void addAgreement(std::shared_ptr<Agreement> agreement) { agreements.push_back(std::move(agreement)); }
	void importAgreements(const std::map<std::string, std::shared_ptr<Country>>& countries);
	[[nodiscard]] const auto& getAgreements() const { return agreements; }
	void updateTagsInAgreements(const std::string& oldTag, const std::string& newTag);

  private:
	void registerKeys();
	std::vector<std::shared_ptr<Agreement>> agreements;
};
} // namespace EU4

#endif // DIPLOMACY_H