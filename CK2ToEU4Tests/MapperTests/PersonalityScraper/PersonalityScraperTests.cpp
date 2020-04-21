#include "../CK2ToEU4/Source/Mappers/PersonalityScraper/PersonalityScraper.h"
#include "gtest/gtest.h"
#include <sstream>

TEST(Mappers_PersonalityScraperTests, personalitiesDefaultToBlank)
{
	std::stringstream input;

	const mappers::PersonalityScraper personalityScraper;

	ASSERT_TRUE(personalityScraper.getPersonalities().empty());
}

TEST(Mappers_PersonalityScraperTests, personalitiesCanBeLoaded)
{
	std::stringstream input;
	input << "personality1 = {}\n";
	input << "personality2 = {}\n";
	input << "personality3 = {}\n";

	mappers::PersonalityScraper personalityScraper;
	personalityScraper.scrapePersonalities(input);

	ASSERT_FALSE(personalityScraper.getPersonalities().empty());
	ASSERT_EQ(personalityScraper.getPersonalities().size(), 3);
}

TEST(Mappers_PersonalityScraperTests, personalitiesAreAssignedAnIncrementalID)
{
	std::stringstream input;
	input << "personality1 = {}\n";
	input << "personality2 = {}\n";
	input << "personality3 = {}\n";

	mappers::PersonalityScraper personalityScraper;
	personalityScraper.scrapePersonalities(input);

	ASSERT_EQ(personalityScraper.getPersonalities().find(1)->second, "personality1");
	ASSERT_EQ(personalityScraper.getPersonalities().find(2)->second, "personality2");
	ASSERT_EQ(personalityScraper.getPersonalities().find(3)->second, "personality3");
}

TEST(Mappers_PersonalityScraperTests, personalitiesCanMatchForID)
{
	std::stringstream input;
	input << "personality1 = {}\n";
	input << "personality2 = {}\n";
	input << "personality3 = {}\n";

	mappers::PersonalityScraper personalityScraper;
	personalityScraper.scrapePersonalities(input);

	ASSERT_EQ(personalityScraper.getPersonalityForID(2), "personality2");
}

TEST(Mappers_PersonalityScraperTests, personalitiesReturnNullforMismatch)
{
	std::stringstream input;
	input << "personality1 = {}\n";
	input << "personality2 = {}\n";
	input << "personality3 = {}\n";

	mappers::PersonalityScraper personalityScraper;
	personalityScraper.scrapePersonalities(input);

	ASSERT_FALSE(personalityScraper.getPersonalityForID(5));
}
