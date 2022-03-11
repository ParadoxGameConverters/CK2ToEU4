#ifndef DIPLOMACY_H
#define DIPLOMACY_H

#include "../../CK2World/Relations/AllRelations.h"
#include "Agreement.h"
#include "Parser.h"
#include <map>
#include <set>
#include <vector>

class Configuration;
namespace EU4
{
class Country;
class Diplomacy: commonItems::parser
{
  public:
	Diplomacy();

	void addAgreement(std::shared_ptr<Agreement> agreement) { agreements.push_back(std::move(agreement)); }
	void importAgreements(const std::map<std::string, std::shared_ptr<Country>>& countries, const CK2::Diplomacy& diplomacy, date conversionDate);
	void importVassals(const std::map<std::string, std::shared_ptr<Country>>& countries, const date& conversionDate);
	void importTributaries(const std::map<std::string, std::shared_ptr<Country>>& countries, const CK2::Diplomacy& diplomacy, date conversionDate);
	void updateTagsInAgreements(const std::string& oldTag, const std::string& newTag);
	void deleteAgreementsWithTag(const std::string& deadTag);
	void filterDeadRelationships(const std::map<std::string, std::shared_ptr<Country>>& countries, const std::set<std::string>& chinaTags);

	[[nodiscard]] const auto& getAgreements() const { return agreements; }
	[[nodiscard]] bool isCountryVassal(const std::string& tag) const;
	[[nodiscard]] bool isCountryJunior(const std::string& tag) const;

  private:
	void registerKeys();

	std::vector<std::shared_ptr<Agreement>> agreements;
};
} // namespace EU4

#endif // DIPLOMACY_H