#ifndef CK2_DYNASTIES_H
#define CK2_DYNASTIES_H
#include "Parser.h"

namespace CK2
{
class Dynasty;
class Dynasties: commonItems::parser
{
  public:
	Dynasties() = default;
	explicit Dynasties(std::istream& theStream); // For testing

	void loadDynasties(const std::filesystem::path& thePath);
	void underLoadDynasties(const std::filesystem::path& thePath);
	void loadDynasties(std::istream& theStream);

	[[nodiscard]] const auto& getDynasties() const { return dynasties; }

  private:
	void registerKeys();
	void registerUnderKeys();

	std::map<int, std::shared_ptr<Dynasty>> dynasties;
};
} // namespace CK2

#endif // CK2_DYNASTIES_H
