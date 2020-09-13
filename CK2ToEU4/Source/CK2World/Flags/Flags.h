#ifndef CK2_FLAGS_H
#define CK2_FLAGS_H
#include "Parser.h"
#include <set>

namespace CK2
{
	class Flags: commonItems::parser
	{
	  public:
		Flags() = default;
		explicit Flags(std::istream& theStream);

		[[nodiscard]] const auto& getFlags() const { return flags; }
		[[nodiscard]] std::set<std::string> fillReformationList();
		[[nodiscard]] bool getInvasion() { return flags.count("aztec_explorers"); }
		[[nodiscard]] bool isGreek() { return greekReformation; }

		[[nodiscard]] bool aztecReformation() { return flags.count("aztec_reformation"); }
		[[nodiscard]] bool balticReformation() { return flags.count("baltic_reformation"); }
		[[nodiscard]] bool bonReformation() { return flags.count("bon_reformation"); }
		[[nodiscard]] bool finnishReformation() { return flags.count("finnish_reformation"); }
		[[nodiscard]] bool hellenicReformation() { return flags.count("hellenic_reformation"); }
		[[nodiscard]] bool norseReformation() { return flags.count("norse_reformation"); }
		[[nodiscard]] bool slavicReformation() { return flags.count("slavic_reformation"); }
		[[nodiscard]] bool tengriReformation() { return flags.count("tengri_reformation"); }
		[[nodiscard]] bool africanReformation() { return flags.count("west_african_reformation"); }
		[[nodiscard]] bool zunReformation() { return flags.count("zun_reformation"); }

	  private:
		void registerKeys();

		bool wasGreek() { return flags.count("flag_hellenic_greek_reformation"); }

		bool greekReformation = false;
		std::set<std::string> flags;
	};

} // namespace CK2

#endif // CK2_FLAGS_H