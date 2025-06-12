#ifndef TITLE_TAG_MAPPER_H
#define TITLE_TAG_MAPPER_H

#include "Parser.h"
#include "TitleTagMapping.h"

namespace mappers
{
class TitleTagMapper: commonItems::parser
{
  public:
	TitleTagMapper() = default;
	void initTitleTagMapper(const std::filesystem::path& path);
	void initTitleTagMapper(std::istream& theStream);										 // testing
	void initTitleTagMapper(std::istream& theStream, std::istream& chineseStream); // testing
	void registerTitle(const std::string& ck2title, const std::string& eu4tag);

	std::optional<std::string> getTagForTitle(const std::string& ck2Title, const std::string& ck2BaseTitle, int eu4Capital);
	std::optional<std::string> getTagForTitle(const std::string& ck2Title, int eu4Capital);
	std::optional<std::string> getTagForTitle(const std::string& ck2Title);
	std::optional<std::string> getChinaForTitle(const std::string& ck2Title);
	std::set<std::string> getAllChinas() const;

	[[nodiscard]] const auto& getMappings() const { return theMappings; }						  // used for testing
	[[nodiscard]] const auto& getRegisteredTitleTags() const { return registeredTitleTags; } // used for testing

  private:
	void registerKeys();
	void registerChineseKeys();
	std::string generateNewTag();

	std::vector<TitleTagMapping> theMappings;
	std::vector<TitleTagMapping> chineseMappings;
	std::map<std::string, std::string> registeredTitleTags; // We store already mapped countries here.
	std::set<std::string> usedTags;

	char generatedEU4TagPrefix = 'Z';
	int generatedEU4TagSuffix = 0;
};
} // namespace mappers

#endif // TITLE_TAG_MAPPER_H