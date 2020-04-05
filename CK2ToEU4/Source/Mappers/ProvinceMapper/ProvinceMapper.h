#ifndef PROVINCE_MAPPER_H
#define PROVINCE_MAPPER_H

#include "newParser.h"
#include <map>
#include <set>
#include "ProvinceMappingsVersion.h"

class Configuration;

namespace mappers
{
	class ProvinceMapper: commonItems::parser
	{
	public:
		ProvinceMapper();
		explicit ProvinceMapper(std::istream& theStream);

		[[nodiscard]] std::vector<int> getCK2ProvinceNumbers(int eu4ProvinceNumber) const;
		[[nodiscard]] std::vector<int> getEU4ProvinceNumbers(int ck2ProvinceNumber) const;
		[[nodiscard]] auto isValidProvince(const int province) const { return validProvinces.count(province) > 0; }

		void determineValidProvinces(const Configuration& theConfiguration);

	private:
		void registerKeys();
		void createMappings();

		std::map<int, std::vector<int>> CK2ToEU4ProvinceMap;
		std::map<int, std::vector<int>> EU4ToCK2ProvinceMap;
		std::set<int> validProvinces;
		ProvinceMappingsVersion theMappings;
	};
}

#endif // PROVINCE_MAPPER_H