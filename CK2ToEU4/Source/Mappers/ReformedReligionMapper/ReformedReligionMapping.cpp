#include "ParserHelpers.h"
#include "ReformedReligionMapping.h"

mappers::ReformedReligionMapping::ReformedReligionMapping()
{

}

mappers::ReformedReligionMapping::ReformedReligionMapping(std::istream& theStream, std::string theFeatures)
{
	parseStream(theStream);
	clearRegisteredKeywords();
}