#ifndef DIPLOMACY_H
#define DIPLOMACY_H

#include "Agreement.h"
#include <map>
#include <vector>

class Configuration;
namespace EU4
{
class Country;
class Diplomacy
{
  public:
	void addAgreement(const Agreement& agreement) { agreements.push_back(agreement); }
	void importAgreements(const std::map<std::string, std::shared_ptr<Country>>& countries);
	void output(const Configuration& theConfiguration) const;

  private:
	std::vector<Agreement> agreements;
};
} // namespace EU4

#endif // DIPLOMACY_H