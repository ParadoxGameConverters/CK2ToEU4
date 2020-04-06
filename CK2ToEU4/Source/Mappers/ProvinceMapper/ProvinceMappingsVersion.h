#ifndef PROVINCE_MAPPING_VERSION_H
#define PROVINCE_MAPPING_VERSION_H

#include "newParser.h"
#include "ProvinceMapping.h"

namespace mappers
{
	class ProvinceMappingsVersion: commonItems::parser
	{
	public:
		ProvinceMappingsVersion() = default;
		ProvinceMappingsVersion(std::istream& theStream);

		[[nodiscard]] const auto& getMappings() const { return mappings; }

	private:
		std::vector<ProvinceMapping> mappings;
	};
}

#endif // PROVINCE_MAPPING_VERSION_H