#ifndef AGREEMENT_H
#define AGREEMENT_H

#include "Date.h"
#include <ostream>

namespace EU4
{
class Agreement
{
  public:
	Agreement(std::string _first, std::string _second, std::string _type, const date& _start_date):
		 type(std::move(_type)), first(std::move(_first)), second(std::move(_second)), start_date(_start_date){}

	[[nodiscard]] const auto& getType() const { return type; }

	friend std::ostream& operator<<(std::ostream& output, const Agreement& agreement);

  private:
	std::string type;
	std::string first;
	std::string second;
	date start_date;
};
} // namespace EU4

#endif // AGREEMENT_H