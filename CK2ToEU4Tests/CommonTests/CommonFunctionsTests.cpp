#include "../../CK2ToEU4/Source/Common/CommonFunctions.h"
#include "gtest/gtest.h"

TEST(CK2World_CommonFunctionsTests, TrimPathTrimsSlashes)
{
	const std::string input = "/this/is/a/path.txt";

	ASSERT_EQ(trimPath(input), "path.txt");
}

TEST(CK2World_CommonFunctionsTests, TrimPathTrimsBackslashes)
{
	const std::string input = "c:\\this\\is\\a\\path.txt";

	ASSERT_EQ(trimPath(input), "path.txt");
}

TEST(CK2World_CommonFunctionsTests, TrimPathTrimsMixedSlashes)
{
	const std::string input = "c:\\this\\is/a/path.txt";

	ASSERT_EQ(trimPath(input), "path.txt");
}

TEST(CK2World_CommonFunctionsTests, TrimExtensionTrimsDot)
{
	const std::string input = "file.extension";

	ASSERT_EQ(trimExtension(input), "file");
}

TEST(CK2World_CommonFunctionsTests, TrimExtensionTrimsLastDot)
{
	const std::string input = "file.name.with.extension";

	ASSERT_EQ(trimExtension(input), "file.name.with");
}

TEST(CK2World_CommonFunctionsTests, ReplaceCharacterCanReplaceSpaces)
{
	const std::string input = "a file name.eu4";

	ASSERT_EQ(replaceCharacter(input, ' '), "a_file_name.eu4");
}

TEST(CK2World_CommonFunctionsTests, ReplaceCharacterCanReplaceMinuses)
{
	const std::string input = "a file-with-name.eu4";

	ASSERT_EQ(replaceCharacter(input, '-'), "a file_with_name.eu4");
}
