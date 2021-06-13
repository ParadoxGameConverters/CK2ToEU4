#ifndef CK2_PROVINCES_H
#define CK2_PROVINCES_H
#include "../../Mappers/MonumentsMapper/MonumentsMapper.h"
#include "../Characters/Character.h"
#include "../Characters/Characters.h"
#include "../Wonders/Wonder.h"
#include "../Wonders/Wonders.h"
#include "Parser.h"

namespace CK2
{
class Province;
class Wonders;
class Provinces: commonItems::parser
{
  public:
	Provinces() = default;
	explicit Provinces(std::istream& theStream);
	[[nodiscard]] const auto& getProvinces() const { return provinces; }


	void linkPrimarySettlements();
	void linkWonders(const Wonders& wonders);																	 // No Leviathan DLC
	std::set<std::string> linkMonuments(const Wonders& wonders, const Characters& characters); // Leviathan DLC

  private:
	void registerKeys();
	void buildMonument(const mappers::MonumentsMapper& monumentsMapper, const std::shared_ptr<CK2::Wonder>& wonder);

	std::map<int, std::shared_ptr<Province>> provinces;
};
} // namespace CK2

#endif // CK2_PROVINCES_H
