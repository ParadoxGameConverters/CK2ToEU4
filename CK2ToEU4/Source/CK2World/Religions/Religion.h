#ifndef CK2_RELIGION_H
#define CK2_RELIGION_H
#include "Parser.h"
#include <set>



namespace CK2
{
	class Religion: commonItems::parser
	{
	  public:
		Religion(std::istream& theStream, const std::string theFeatures);
		[[nodiscard]] const auto& getFeatures() const { return features; }

	  private:
		void registerKeys();

		std::vector<std::string> features;
	};
} // namespace CK2

#endif // CK2_RELIGIONS_H
