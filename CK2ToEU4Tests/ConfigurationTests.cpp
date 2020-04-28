#include "../CK2ToEU4/Source/Configuration/Configuration.h"
#include "gtest/gtest.h"
#include <sstream>

TEST(CK2ToEU4_ConfigurationTests, EU4PathDefaultsBlank)
{
	std::stringstream input("");
	const Configuration testConfiguration(input);

	ASSERT_EQ(testConfiguration.getEU4Path(), "");
}

TEST(CK2ToEU4_ConfigurationTests, EU4PathCanBeSet)
{
	std::stringstream input;
	input << "EU4directory = \"C:\\EU4Path\"";
	const Configuration testConfiguration(input);

	ASSERT_EQ(testConfiguration.getEU4Path(), "C:\\EU4Path");
}

TEST(CK2ToEU4_ConfigurationTests, CK2PathDefaultsBlank)
{
	std::stringstream input("");
	const Configuration testConfiguration(input);

	ASSERT_EQ(testConfiguration.getCK2Path(), "");
}

TEST(CK2ToEU4_ConfigurationTests, CK2PathCanBeSet)
{
	std::stringstream input;
	input << "CK2directory = \"C:\\CK2Path\"";
	const Configuration testConfiguration(input);

	ASSERT_EQ(testConfiguration.getCK2Path(), "C:\\CK2Path");
}

TEST(CK2ToEU4_ConfigurationTests, CK2ModsPathDefaultsBlank)
{
	std::stringstream input("");
	const Configuration testConfiguration(input);

	ASSERT_EQ(testConfiguration.getCK2ModsPath(), "");
}

TEST(CK2ToEU4_ConfigurationTests, CK2ModsPathCanBeSet)
{
	std::stringstream input;
	input << "CK2ModsDirectory = \"C:\\CK2Path\\Mods\"";
	const Configuration testConfiguration(input);

	ASSERT_EQ(testConfiguration.getCK2ModsPath(), "C:\\CK2Path\\Mods");
}

TEST(CK2ToEU4_ConfigurationTests, SaveGamePathDefaultsBlank)
{
	std::stringstream input("");
	const Configuration testConfiguration(input);

	ASSERT_EQ(testConfiguration.getSaveGamePath(), "");
}

TEST(CK2ToEU4_ConfigurationTests, SaveGamePathCanBeSet)
{
	std::stringstream input;
	input << "SaveGame = \"C:\\CK2Path\\save games\\autosave.ck2\"";
	const Configuration testConfiguration(input);

	ASSERT_EQ(testConfiguration.getSaveGamePath(), "C:\\CK2Path\\save games\\autosave.ck2");
}

TEST(CK2ToEU4_ConfigurationTests, OutputNameDefaultsToSaveGameWithNoOutputName)
{
	std::stringstream input;
	input << "SaveGame = \"C:\\CK2Path\\save games\\autosave.ck2\"";

	const Configuration testConfiguration(input);

	ASSERT_EQ(testConfiguration.getOutputName(), "autosave");
}

TEST(CK2ToEU4_ConfigurationTests, OutputNameResistantToMixedSlashes)
{
	std::stringstream input;
	input << "SaveGame = \"C:\\CK2Path/save games/autosave.ck2\"";
	const Configuration testConfiguration(input);

	ASSERT_EQ(testConfiguration.getOutputName(), "autosave");
}

TEST(CK2ToEU4_ConfigurationTests, OutputNameReplacesSpacesAndMinuses)
{
	std::stringstream input;
	input << "SaveGame = \"C:\\CK2Path\\save games\\autosav - - . second e.ck2\"";

	const Configuration testConfiguration(input);

	ASSERT_EQ(testConfiguration.getOutputName(), "autosav_____._second_e");
}

TEST(CK2ToEU4_ConfigurationTests, OutputNameDefaultsToSaveGame)
{
	std::stringstream input;
	input << "output_name = \"\"\n";
	input << "SaveGame = \"C:\\CK2Path\\save games\\autosave.ck2\"";

	const Configuration testConfiguration(input);

	ASSERT_EQ(testConfiguration.getOutputName(), "autosave");
}

TEST(CK2ToEU4_ConfigurationTests, OutputNameCanBeSet)
{
	std::stringstream input;
	input << "output_name = \"override\"\n";
	input << "SaveGame = \"C:\\CK2Path\\save games\\autosave.ck2\"";

	const Configuration testConfiguration(input);

	ASSERT_EQ(testConfiguration.getOutputName(), "override");
}
