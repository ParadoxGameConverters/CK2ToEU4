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
		bool getInvasion() { return flags.count("aztec_explorers"); }
		bool isGreek() { return greekReformation; }

		bool aztecReformation() { return flags.count("aztec_reformation"); }
		bool balticReformation() { return flags.count("baltic_reformation"); }
		bool bonReformation() { return flags.count("bon_reformation"); }
		bool finnishReformation() { return flags.count("finnish_reformation"); }
		bool hellenicReformation() { return flags.count("hellenic_reformation"); }
		bool norseReformation() { return flags.count("norse_reformation"); }
		bool slavicReformation() { return flags.count("slavic_reformation"); }
		bool tengriReformation() { return flags.count("tengri_reformation"); }
		bool africanReformation() { return flags.count("west_african_reformation"); }
		bool zunReformation() { return flags.count("zun_reformation"); }

	  private:
		void registerKeys();

		bool wasGreek() { return flags.count("flag_hellenic_greek_reformation"); }

		bool greekReformation = false;
		std::set<std::string> flags;
	};

} // namespace CK2

#endif // CK2_FLAGS_H