#include "World.h"
#include "../Configuration/Configuration.h"
#include "Characters/Character.h"
#include "CommonFunctions.h"
#include "CommonRegexes.h"
#include "Date.h"
#include "GameVersion.h"
#include "Log.h"
#include "OSCompatibilityLayer.h"
#include "Offmaps/Offmap.h"
#include "ParserHelpers.h"
#include "Religions/Religions.h"
#include "Titles/Liege.h"
#include "Titles/Title.h"
#include <ZipFile.h>
#include <cmath>
#include <filesystem>
#include <fstream>
namespace fs = std::filesystem;

CK2::World::World(const Configuration& theConfiguration, const commonItems::ConverterVersion& converterVersion)
{
	Log(LogLevel::Info) << "*** Hello CK2, Deus Vult! ***";
	registerKeyword("CK2txt", [](const std::string& unused, std::istream& theStream) {
	});
	registerKeyword("date", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString dateString(theStream);
		endDate = date(dateString.getString());
	});
	registerKeyword("start_date", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString startDateString(theStream);
		startDate = date(startDateString.getString());
	});
	registerKeyword("flags", [this](const std::string& unused, std::istream& theStream) {
		Log(LogLevel::Info) << "-> Loading Flags";
		flags = Flags(theStream);
		Log(LogLevel::Info) << ">> Loaded " << flags.getFlags().size() << " Global Flags.";
	});
	registerKeyword("version", [this, converterVersion](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString versionString(theStream);
		CK2Version = GameVersion(versionString.getString());
		Log(LogLevel::Info) << "<> Savegame version: " << versionString.getString();

		if (converterVersion.getMinSource() > CK2Version)
		{
			Log(LogLevel::Error) << "Converter requires a minimum save from v" << converterVersion.getMinSource().toShortString();
			throw std::runtime_error("Savegame vs converter version mismatch!");
		}
		if (!converterVersion.getMaxSource().isLargerishThan(CK2Version))
		{
			Log(LogLevel::Error) << "Converter requires a maximum save from v" << converterVersion.getMaxSource().toShortString();
			throw std::runtime_error("Savegame vs converter version mismatch!");
		}
	});
	registerKeyword("provinces", [this](const std::string& unused, std::istream& theStream) {
		Log(LogLevel::Info) << "-> Loading Provinces";
		provinces = Provinces(theStream);
		Log(LogLevel::Info) << ">> Loaded " << provinces.getProvinces().size() << " provinces.";
	});
	registerKeyword("character", [this](const std::string& unused, std::istream& theStream) {
		Log(LogLevel::Info) << "-> Loading Characters";
		characters = Characters(theStream);
		Log(LogLevel::Info) << ">> Loaded " << characters.getCharacters().size() << " characters.";
	});
	registerKeyword("title", [this](const std::string& unused, std::istream& theStream) {
		Log(LogLevel::Info) << "-> Loading Titles";
		titles = Titles(theStream);
		Log(LogLevel::Info) << ">> Loaded " << titles.getTitles().size() << " titles.";
	});
	registerKeyword("religion", [this](const std::string& unused, std::istream& theStream) {
		Log(LogLevel::Info) << "-> Loading Religions";
		religions = Religions(theStream);
		Log(LogLevel::Info) << ">> Loaded " << religions.getReformedReligion().size() << " Reformed Religions.";
	});
	registerKeyword("dynasties", [this](const std::string& unused, std::istream& theStream) {
		Log(LogLevel::Info) << "-> Loading Dynasties";
		dynasties.loadDynasties(theStream);
		Log(LogLevel::Info) << ">> Loaded " << dynasties.getDynasties().size() << " dynasties.";
	});
	registerKeyword("wonder", [this](const std::string& unused, std::istream& theStream) {
		Log(LogLevel::Info) << "-> Loading Wonders";
		wonders = Wonders(theStream);
		Log(LogLevel::Info) << ">> Loaded " << wonders.getWonders().size() << " wonders.";
	});
	registerKeyword("offmap_powers", [this](const std::string& unused, std::istream& theStream) {
		Log(LogLevel::Info) << "-> Loading Offmaps";
		offmaps = Offmaps(theStream);
		Log(LogLevel::Info) << ">> Loaded " << offmaps.getOffmaps().size() << " offmaps.";
	});
	registerKeyword("dyn_title", [this](const std::string& unused, std::istream& theStream) {
		const auto dynTitle = Liege(theStream);
		dynamicTitles.insert(std::pair(dynTitle.getTitle().first, dynTitle));
	});
	registerKeyword("relation", [this](const std::string& unused, std::istream& theStream) {
		Log(LogLevel::Info) << "-> Loading Diplomacy";
		diplomacy = Diplomacy(theStream);
		Log(LogLevel::Info) << ">> Loaded " << diplomacy.getDiplomacy().size() << " personal diplomacies.";
	});
	registerKeyword("vars", [this](const std::string& unused, std::istream& theStream) {
		Log(LogLevel::Info) << "-> Loading Variables";
		vars = Vars(theStream);
		Log(LogLevel::Info) << ">> Loaded " << vars.getVars().size() << " global variables.";
	});

	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
	Log(LogLevel::Progress) << "4 %";

	Log(LogLevel::Info) << "-> Verifying CK2 save.";
	verifySave(theConfiguration.getSaveGamePath());

	Log(LogLevel::Info) << "-> Importing CK2 save.";
	if (!saveGame.compressed)
	{
		std::ifstream inBinary(fs::u8path(theConfiguration.getSaveGamePath()), std::ios::binary);
		if (!inBinary.is_open())
		{
			Log(LogLevel::Error) << "Could not open " << theConfiguration.getSaveGamePath() << " for parsing.";
			throw std::runtime_error("Could not open " + theConfiguration.getSaveGamePath() + " for parsing.");
		}
		std::stringstream inStream;
		inStream << inBinary.rdbuf();
		saveGame.gamestate = inStream.str();
	}

	Log(LogLevel::Info) << "-> Locating mods in mod folder";
	commonItems::ModLoader modLoader;
	modLoader.loadMods(theConfiguration.getCK2DocsPath(), theConfiguration.getMods());
	mods = modLoader.getMods();
	Log(LogLevel::Progress) << "6 %";

	// We must load initializers before the savegame.
	// Do we have an override mod?
	std::string overrideModPath;
	for (const auto& mod: mods)
		if (mod.name == "CleanSlate")
			overrideModPath = "CleanSlate";
	reformedReligionMapper.initReformedReligionMapper(overrideModPath);
	loadDynasties(theConfiguration);
	Log(LogLevel::Progress) << "7 %";

	personalityScraper.scrapePersonalities(theConfiguration);
	Log(LogLevel::Progress) << "8 %";

	auto gameState = std::istringstream(saveGame.gamestate);
	parseStream(gameState);
	Log(LogLevel::Progress) << "10 %";
	clearRegisteredKeywords();
	Log(LogLevel::Info) << ">> Loaded " << dynamicTitles.size() << " dynamic titles.";
	Log(LogLevel::Info) << "-> Importing Province Titles";
	loadProvinces(theConfiguration);
	Log(LogLevel::Progress) << "11 %";
	Log(LogLevel::Info) << "-> Setting Flags";
	invasion = flags.getInvasion();					  // Sunset Invasion
	reformationList = flags.fillReformationList(); // Reformed Pagans
	if (flags.hellenicReformation())
		greekReformation = flags.isGreek(); // Were Hellenes Greek or Roman?
	Log(LogLevel::Progress) << "12 %";

	Log(LogLevel::Info) << "*** Building World ***";

	// Link all the intertwining pointers
	Log(LogLevel::Info) << "-- Filtering Excess Province Titles";
	provinceTitleMapper.filterSelf(provinces, titles);
	Log(LogLevel::Progress) << "13 %";
	Log(LogLevel::Info) << "-- Linking Characters With Dynasties";
	characters.linkDynasties(dynasties);
	Log(LogLevel::Progress) << "14 %";
	Log(LogLevel::Info) << "-- Linking Characters With Lieges and Spouses";
	characters.linkLiegesAndSpouses();
	Log(LogLevel::Progress) << "15 %";
	Log(LogLevel::Info) << "-- Linking Characters With Family";
	characters.linkMothersAndFathers();
	Log(LogLevel::Progress) << "16 %";
	Log(LogLevel::Info) << "-- Linking Characters With Primary Titles";
	characters.linkPrimaryTitles(titles);
	Log(LogLevel::Progress) << "17 %";
	Log(LogLevel::Info) << "-- Linking Characters With Capitals";
	characters.linkCapitals(provinces);
	Log(LogLevel::Progress) << "18 %";
	Log(LogLevel::Info) << "-- Linking Provinces With Primary Baronies";
	provinces.linkPrimarySettlements();
	Log(LogLevel::Progress) << "19 %";
	Log(LogLevel::Info) << "-- Linking Provinces With Wonders";
	leviathanDLC = commonItems::DoesFileExist(theConfiguration.getEU4Path() + "/dlc/dlc106_leviathan/dlc106.dlc");
	if (isLeviathanDLCPresent())
		setExistentPremadeMonuments(provinces.linkMonuments(wonders, characters));
	else
		provinces.linkWonders(wonders);
	Log(LogLevel::Progress) << "20 %";
	Log(LogLevel::Info) << "-- Linking Titles With Holders";
	titles.linkHolders(characters);
	Log(LogLevel::Progress) << "21 %";
	Log(LogLevel::Info) << "-- Linking Titles With Previous Holders";
	titles.linkPreviousHolders(characters);
	Log(LogLevel::Progress) << "22 %";
	Log(LogLevel::Info) << "-- Linking Titles With Liege and DeJure Titles";
	titles.linkLiegePrimaryTitles();
	Log(LogLevel::Progress) << "23 %";
	Log(LogLevel::Info) << "-- Linking Titles With Vassals and DeJure Vassals";
	titles.linkVassals();
	Log(LogLevel::Progress) << "24 %";
	Log(LogLevel::Info) << "-- Linking Titles With Provinces";
	titles.linkProvinces(provinces, provinceTitleMapper); // Untestable due to disk access.
	Log(LogLevel::Progress) << "25 %";
	Log(LogLevel::Info) << "-- Linking Titles With Base Titles";
	titles.linkBaseTitles();
	Log(LogLevel::Progress) << "26 %";
	Log(LogLevel::Info) << "-- Linking The Celestial Emperor";
	linkCelestialEmperor();
	Log(LogLevel::Progress) << "30 %";
	Log(LogLevel::Info) << "-- Creating Reformed Religions";
	createReformedFeatures();
	Log(LogLevel::Progress) << "31 %";

	// Filter top-tier active titles and assign them provinces.

	Log(LogLevel::Info) << "-- Merging Independent Baronies";
	mergeIndependentBaronies();
	Log(LogLevel::Progress) << "32 %";
	Log(LogLevel::Info) << "-- Merging Revolts Into Base";
	titles.mergeRevolts();
	Log(LogLevel::Progress) << "33 %";
	Log(LogLevel::Info) << "-- Flagging HRE Provinces";
	flagHREProvinces(theConfiguration);
	Log(LogLevel::Info) << "-- Shattering HRE";
	shatterHRE(theConfiguration);
	Log(LogLevel::Progress) << "34 %";
	Log(LogLevel::Info) << "-- Shattering Empires";
	shatterEmpires(theConfiguration);
	Log(LogLevel::Progress) << "35 %";
	Log(LogLevel::Info) << "-- Filtering Independent Titles";
	filterIndependentTitles();
	Log(LogLevel::Progress) << "36 %";
	Log(LogLevel::Info) << "-- Splitting Off Vassals";
	splitVassals(theConfiguration);
	Log(LogLevel::Progress) << "37 %";
	Log(LogLevel::Info) << "-- Rounding Up Some People";
	gatherCourtierNames();
	Log(LogLevel::Progress) << "38 %";
	Log(LogLevel::Info) << "-- Congregating Provinces for Independent Titles";
	congregateProvinces();
	Log(LogLevel::Progress) << "39 %";
	Log(LogLevel::Info) << "-- Distributing Electorates";
	linkElectors();
	Log(LogLevel::Progress) << "40 %";
	Log(LogLevel::Info) << "-- Congregating DeJure Provinces for Independent Titles";
	congregateDeJureProvinces();
	Log(LogLevel::Progress) << "41 %";
	Log(LogLevel::Info) << "-- Performing Province Sanity Check";
	sanityCheckifyProvinces();
	Log(LogLevel::Progress) << "42 %";
	Log(LogLevel::Info) << "-- Filtering Provinceless Titles";
	filterProvincelessTitles();
	Log(LogLevel::Progress) << "43 %";
	Log(LogLevel::Info) << "-- Determining Heirs";
	determineHeirs();
	Log(LogLevel::Progress) << "44 %";
	Log(LogLevel::Info) << "-- Decyphering Personalities";
	characters.assignPersonalities(personalityScraper);
	Log(LogLevel::Progress) << "45 %";
	alterSunset(theConfiguration);
	Log(LogLevel::Info) << "*** Good-bye CK2, rest in peace. ***";
	Log(LogLevel::Progress) << "47 %";
}

void CK2::World::loadDynasties(const Configuration& theConfiguration)
{
	auto fileNames = commonItems::GetAllFilesInFolder(theConfiguration.getCK2Path() + "/common/dynasties/");
	for (const auto& file: fileNames)
		dynasties.loadDynasties(theConfiguration.getCK2Path() + "/common/dynasties/" + file);
	for (const auto& mod: mods)
	{
		fileNames = commonItems::GetAllFilesInFolder(mod.path + "/common/dynasties/");
		for (const auto& file: fileNames)
		{
			if (file.find(".txt") == std::string::npos)
				continue;
			Log(LogLevel::Info) << "\t>> Loading additional dynasties from [" << mod.name << "]: " << mod.path + "/common/dynasties/" + file;
			dynasties.loadDynasties(mod.path + "/common/dynasties/" + file);
		}
	}
}

void CK2::World::loadProvinces(const Configuration& theConfiguration)
{
	// Vanilla has errors and mismatches. Mods have targeted expansions and replacements. This means for vanilla we have to load a multimap,
	// but for mods we're overwriting all multimap matches.
	provinceTitleMapper.loadProvinces(theConfiguration.getCK2Path());

	for (const auto& mod: mods)
	{
		if (commonItems::DoesFolderExist(mod.path + "/history/provinces/"))
		{
			Log(LogLevel::Info) << "\t>> Loading additional provinces from [" << mod.name << "]: " << mod.path + "/history/provinces/";
			provinceTitleMapper.updateProvinces(mod.path);
		}
	}
}

void CK2::World::linkElectors()
{
	// Finding electorates is not entirely trivial. CK2 has 8 slots, one of which is usually the Emperor himself, but
	// he is not considered an elector in EU4 sense unless he holds one of the electorate titles which are historical.
	// However, CK2 doesn't care about titles, it stores people, so a multiduke with a secondary electoral title will still
	// be elector and we need to flag his primary title as electorate one, as other duchies will possibly be annexed or PU-d.
	// Moreover, these electors may not even be indeps after HRE shattering as player may opt to keep kingdoms but electors were
	// under these kings. We can't help that.

	const auto& allTitles = titles.getTitles();
	const auto& hre = allTitles.find("e_hre");
	if (hre == allTitles.end())
	{
		Log(LogLevel::Info) << ">< HRE does not exist.";
		return;
	}
	auto electors = hre->second->getElectors();
	if (electors.empty())
	{
		Log(LogLevel::Info) << ">< HRE does not have electors.";
		return;
	}
	if (electors.size() > 8)
	{
		Log(LogLevel::Info) << ">< HRE has too many electors.";
		return;
	}
	if (hre->second->getSuccessionLaw() != "feudal_elective")
	{
		Log(LogLevel::Info) << ">< HRE does not have feudal elective succession law.";
		return;
	}

	const auto& allCharacters = characters.getCharacters();
	auto counter = 0;

	// Preambule done, we start here.
	// Make a registry of indep titles and their holders.
	std::map<int, std::map<std::string, std::shared_ptr<Title>>> holderTitles; // holder/titles
	std::pair<int, std::shared_ptr<Character>> hreHolder;

	for (const auto& title: independentTitles)
	{
		if (title.second->getHolder().first)
		{
			holderTitles[title.second->getHolder().first].insert(title);
			if (title.second->isHREEmperor())
			{
				hreHolder = title.second->getHolder();
			}
		}
	}

	if (!hreHolder.first)
	{
		Log(LogLevel::Info) << ">< HRE does not have an emperor.";
		return;
	}

	// Now roll through electors and flag their primary titles as electors. If kings get electorate status
	// but kingdoms are also shattered, tough luck? Their primary duchy won't inherit electorate as they could
	// end up with multiple electorates, and that's possible only through EU4 gameplay and causes massive
	// penalties to IA.

	for (auto& elector: electors)
	{
		if (electors.size() > 7 && elector == hreHolder.first)
		{
			continue; // We're skipping the emperor for 8-slot setups.
		}

		const auto& charItr = allCharacters.find(elector);
		if (charItr == allCharacters.end())
		{
			continue; // Non-existent fellow?
		}

		// How many indep titles does he hold? If any?
		const auto& regItr = holderTitles.find(elector);
		if (regItr == holderTitles.end())
		{
			continue; // This fellow was cheated out of electorate titles.
		}

		if (regItr->second.size() > 1)
		{
			// Which title is his primary?
			const auto& primaryTitle = charItr->second->getPrimaryTitle();
			if (primaryTitle.first.empty())
			{
				// We have a person without a primary title, possibly an ex-king, holding multiple actual titles.
				// To make matters worse, he may not even be of a religion allowing PUs.
				// Well, grab the first one with some land.
				for (const auto& title: regItr->second)
				{
					if (!title.second->getProvinces().empty())
					{
						title.second->setElectorate();
						counter++;
						break;
					}
				}
				// otherwise no helping this fellow.
			}
			else
			{
				primaryTitle.second->getTitle().second->setElectorate();
				counter++;
			}
		}
		else
		{
			regItr->second.begin()->second->setElectorate();
			counter++;
		}
	}

	Log(LogLevel::Info) << "<> " << counter << " electorates linked.";
}

void CK2::World::alterSunset(const Configuration& theConfiguration)
{
	if (theConfiguration.getSunset() == Configuration::SUNSET::ACTIVE)
		invasion = true;
	else if (theConfiguration.getSunset() == Configuration::SUNSET::DISABLED)
		invasion = false;
}

void CK2::World::linkCelestialEmperor() const
{
	const auto& china = offmaps.getChina();
	if (!china)
	{
		Log(LogLevel::Info) << ">< No China detected.";
		return;
	}
	if (!china->second->getHolder().first)
	{
		Log(LogLevel::Info) << ">< China has no emperor.";
		return;
	}
	const auto& chars = characters.getCharacters();
	const auto& characterItr = chars.find(china->second->getHolder().first);
	if (characterItr == chars.end())
	{
		Log(LogLevel::Info) << ">< Celestial emperor has no definition!";
		return;
	}
	china->second->setHolder(std::pair(characterItr->first, characterItr->second));
	const auto& holder = china->second->getHolder();
	if (!holder.second->getDynasty().first)
	{
		Log(LogLevel::Info) << ">< Celestial emperor has no dynasty!";
		return;
	}
	const auto& dyns = dynasties.getDynasties();
	const auto& dynastyItr = dyns.find(holder.second->getDynasty().first);
	if (dynastyItr == dyns.end())
	{
		Log(LogLevel::Info) << ">< Celestial emperor's dynasty has no definition!";
		return;
	}
	holder.second->setDynasty(dynastyItr->second);
	Log(LogLevel::Info) << "<> One Celestial Emperor linked.";
}

void CK2::World::determineHeirs()
{
	// We're doing this one late as the number of people involved is reduced by thousandfold.
	for (const auto& title: independentTitles)
	{
		const auto& holder = title.second->getHolder();
		auto law = title.second->getSuccessionLaw();
		auto gender = title.second->getGenderLaw();

		if (law == "primogeniture" || law == "elective_gavelkind" || law == "gavelkind" || law == "nomad_succession")
			resolvePrimogeniture(gender, holder);
		else if (law == "ultimogeniture")
			resolveUltimogeniture(gender, holder);
		else if (law == "tanistry" || law == "eldership")
			resolveTanistry(gender, holder);
		else if (law == "turkish_succession")
			resolveTurkish(holder);
	}
	Log(LogLevel::Info) << "<> Heirs resolved where possible.";
}

void CK2::World::resolveTurkish(const std::pair<int, std::shared_ptr<Character>>& holder) const
{
	auto children = holder.second->getChildren();
	std::vector<std::pair<int, std::shared_ptr<Character>>> childVector;

	// instead of filtering by id, we're filtering by raw prestige.
	for (const auto& child: children)
		childVector.emplace_back(std::pair(lround(child.second->getPrestige()), child.second));
	std::sort(childVector.begin(), childVector.end());

	for (const auto& child: childVector)
	{
		if (child.second->getDeathDate() != date("1.1.1"))
			continue;
		holder.second->setHeir(std::pair(child.second->getID(), child.second));
		return;
	}
}

void CK2::World::resolveTanistry(const std::string& genderLaw, const std::pair<int, std::shared_ptr<Character>>& holder) const
{
	// We have no clue who a tanistry successor might be.
	// Such luck! It's the uncle/aunt the son/daughter was named after!
	resolvePrimogeniture(genderLaw, holder);
	const auto& heir = holder.second->getHeir();
	if (heir.first)
		heir.second->addYears(35);
}

void CK2::World::resolvePrimogeniture(const std::string& genderLaw, const std::pair<int, std::shared_ptr<Character>>& holder) const
{
	auto children = holder.second->getChildren();

	// Using the awesome knowledge that a smaller ID means earlier character, we don't have to sort them by age.
	std::vector<std::pair<int, std::shared_ptr<Character>>> childVector;
	for (const auto& child: children)
		childVector.emplace_back(child);
	std::sort(childVector.begin(), childVector.end());

	std::pair<int, std::shared_ptr<Character>> son;		  // primary heir candidate
	std::pair<int, std::shared_ptr<Character>> daughter; // primary heir candidate

	for (const auto& child: childVector)
	{
		if (child.second->getDeathDate() != date("1.1.1"))
			continue; // Dead.
		if (!son.first && !child.second->isFemale())
			son = child;
		if (son.first && !child.second->isFemale() && son.first == child.first - 1)
			son = child; // Ask paradox. Seriously.
		if (!daughter.first && child.second->isFemale())
			daughter = child;
		if (daughter.first && child.second->isFemale() && daughter.first == child.first - 1)
			daughter = child; // Twins have reversed IDs, yay!
	}

	if ((genderLaw == "agnatic" || genderLaw == "cognatic") && son.first)
	{
		holder.second->setHeir(son);
		return;
	}
	if (genderLaw == "cognatic" && daughter.first)
	{
		holder.second->setHeir(daughter);
		return;
	}
	if (genderLaw == "true_cognatic" && (son.first || daughter.first))
	{
		if (son.first && daughter.first)
		{
			if (son.first < daughter.first)
				holder.second->setHeir(son);
			else
				holder.second->setHeir(daughter);

			// Stop! Sanity police!
			if (son.first == daughter.first - 1)
				holder.second->setHeir(daughter);
			if (daughter.first == son.first - 1)
				holder.second->setHeir(son);
			// You are insane, you may proceed.
		}
		else if (son.first)
			holder.second->setHeir(son);
		else
			holder.second->setHeir(daughter);
	}
}

void CK2::World::resolveUltimogeniture(const std::string& genderLaw, const std::pair<int, std::shared_ptr<Character>>& holder) const
{
	auto children = holder.second->getChildren();
	std::vector<std::pair<int, std::shared_ptr<Character>>> childVector;
	for (const auto& child: children)
		childVector.emplace_back(child);
	std::sort(childVector.rbegin(), childVector.rend());
	std::pair<int, std::shared_ptr<Character>> son;
	std::pair<int, std::shared_ptr<Character>> daughter;
	for (const auto& child: childVector)
	{
		if (child.second->getDeathDate() != date("1.1.1"))
			continue;
		if (!son.first && !child.second->isFemale())
			son = child;
		if (!daughter.first && child.second->isFemale())
			daughter = child;
	}

	if ((genderLaw == "agnatic" || genderLaw == "cognatic") && son.first)
	{
		holder.second->setHeir(son);
		return;
	}
	if (genderLaw == "cognatic" && daughter.first)
	{
		holder.second->setHeir(daughter);
		return;
	}
	if (genderLaw == "true_cognatic" && (son.first || daughter.first))
	{
		if (son.first && daughter.first)
		{
			if (son.first < daughter.first)
				holder.second->setHeir(son);
			else
				holder.second->setHeir(daughter);
		}
		else if (son.first)
			holder.second->setHeir(son);
		else
			holder.second->setHeir(daughter);
	}
}


void CK2::World::gatherCourtierNames()
{
	// We're using this function to Locate courtiers, assemble their names as potential Monarch Names in EU4,
	// and also while at it, to see if they hold adviser jobs.

	auto counter = 0;
	auto counterAdvisors = 0;
	std::map<int, std::map<std::string, bool>> holderCourtiers;					 // holder-name/male
	std::map<int, std::map<int, std::shared_ptr<Character>>> holderAdvisors; // holder-advisors

	for (const auto& character: characters.getCharacters())
	{
		if (character.second->getHost())
		{
			holderCourtiers[character.second->getHost()].insert(std::pair(character.second->getName(), !character.second->isFemale()));
			if (!character.second->getJob().empty())
				holderAdvisors[character.second->getHost()].insert(character);
		}
	}
	for (const auto& title: independentTitles)
	{
		if (title.second->getHolder().first)
		{
			const auto containerItr = holderCourtiers.find(title.second->getHolder().first);
			if (containerItr != holderCourtiers.end())
			{
				title.second->getHolder().second->setCourtierNames(containerItr->second);
				counter += static_cast<int>(containerItr->second.size());
			}
			const auto adviserItr = holderAdvisors.find(title.second->getHolder().first);
			if (adviserItr != holderAdvisors.end())
			{
				title.second->getHolder().second->setAdvisers(adviserItr->second);
				counterAdvisors += static_cast<int>(adviserItr->second.size());
			}
		}
	}
	Log(LogLevel::Info) << "<> " << counter << " people gathered for interrogation. " << counterAdvisors << " were detained.";
}

void CK2::World::splitVassals(const Configuration& theConfiguration)
{
	if (theConfiguration.getSplitVassals() == Configuration::SPLITVASSALS::NO)
	{
		Log(LogLevel::Info) << ">< Splitting vassals disabled by configuration.";
		return;
	}

	std::map<std::string, std::shared_ptr<Title>> newIndeps;

	// We have linked counties to provinces, and we know who's independent.
	// We can now go through all titles and see what should be an independent vassal.
	for (const auto& title: independentTitles)
	{
		if (title.second->isThePope() || title.second->isTheFraticelliPope() || title.first == "e_outremer" || title.first == "e_china_west_governor")
			continue; // Not touching these.
		// let's not split hordes or tribals.
		if (title.second->getHolder().second->getGovernment() == "tribal_government" || title.second->getHolder().second->getGovernment() == "nomadic_government")
			continue;
		auto relevantVassals = 0;
		std::string relevantVassalPrefix;
		if (title.first.find("e_") == 0)
			relevantVassalPrefix = "k_";
		else if (title.first.find("k_") == 0)
			relevantVassalPrefix = "d_";
		else
			continue; // Not splitting off counties.
		for (const auto& vassal: title.second->getVassals())
		{
			if (vassal.first.find(relevantVassalPrefix) != 0)
				continue; // they are not relevant
			if (vassal.second->coalesceProvinces().empty())
				continue; // no land, not relevant
			relevantVassals++;
		}
		if (!relevantVassals)
			continue;																		// no need to split off anything.
		const auto& provincesClaimed = title.second->coalesceProvinces(); // this is our primary total.
		for (const auto& vassal: title.second->getVassals())
		{
			if (vassal.first.find(relevantVassalPrefix) != 0)
				continue; // they are not relevant
			if (vassal.second->getHolder().first == title.second->getHolder().first)
				continue; // Not splitting our own land.
			const auto& vassalProvincesClaimed = vassal.second->coalesceProvinces();

			// a vassal goes indep if they (control 1/relevantvassals + 10% land) * factor defined in vassal_splitoff.txt.
			double threshold = static_cast<double>(provincesClaimed.size()) / relevantVassals + 0.1 * static_cast<double>(provincesClaimed.size());
			threshold *= vassalSplitoffMapper.getFactor();
			if (static_cast<double>(vassalProvincesClaimed.size()) > threshold)
				newIndeps.insert(vassal);
		}
	}

	// Now let's free them.
	for (const auto& newIndep: newIndeps)
	{
		const auto& liege = newIndep.second->getLiege().second->getTitle();
		liege.second->registerGeneratedVassal(newIndep);
		newIndep.second->registerGeneratedLiege(liege);
		independentTitles.insert(newIndep);
	}
	Log(LogLevel::Info) << "<> " << newIndeps.size() << " vassals liberated from immediate integration.";
}


void CK2::World::verifySave(const std::string& saveGamePath)
{
	std::ifstream saveFile(fs::u8path(saveGamePath));
	if (!saveFile.is_open())
		throw std::runtime_error("Could not open save! Exiting!");

	char buffer[3];
	saveFile.get(buffer, 3);
	if (buffer[0] == 'P' && buffer[1] == 'K')
	{
		if (!uncompressSave(saveGamePath))
			throw std::runtime_error("Failed to unpack the compressed save!");
		saveGame.compressed = true;
	}
	saveFile.close();
}

bool CK2::World::uncompressSave(const std::string& saveGamePath)
{
	auto savefile = ZipFile::Open(saveGamePath);
	if (!savefile)
		return false;
	for (size_t entryNum = 0; entryNum < savefile->GetEntriesCount(); ++entryNum)
	{
		const auto& entry = savefile->GetEntry(static_cast<int>(entryNum));
		const auto& name = entry->GetName();
		if (name == "meta")
		{
			Log(LogLevel::Info) << ">> Uncompressing metadata";
			saveGame.metadata = std::string{std::istreambuf_iterator<char>(*entry->GetDecompressionStream()), std::istreambuf_iterator<char>()};
		}
		else if (getExtension(saveGamePath) == "ck2")
		{
			Log(LogLevel::Info) << ">> Uncompressing gamestate";
			saveGame.gamestate = std::string{std::istreambuf_iterator<char>(*entry->GetDecompressionStream()), std::istreambuf_iterator<char>()};
		}
		else
		{
			throw std::runtime_error("Unrecognized savegame structure! What is this file: " + name);
		}
	}
	return true;
}

void CK2::World::filterIndependentTitles()
{
	const auto& allTitles = titles.getTitles();
	std::map<std::string, std::shared_ptr<Title>> potentialIndeps;

	for (const auto& title: allTitles)
	{
		const auto& liege = title.second->getLiege();
		const auto& holder = title.second->getHolder();
		if (!holder.first)
			continue; // don't bother with titles without holders.
		if (liege.first.empty())
		{
			// this is a potential indep.
			potentialIndeps.insert(std::pair(title.first, title.second));
		}
	}

	// Check if we hold any actual land (b|c_something). (Only necessary for the holder,
	// no need to recurse, we're just filtering landless titular titles like mercenaries
	// or landless Pope. If a character holds a landless titular title along actual title
	// (like Caliphate), it's not relevant at this stage as he's independent anyway.

	// First, split off all county_title holders into a container.
	std::set<int> countyHolders;
	std::map<int, std::map<std::string, std::shared_ptr<Title>>> allTitleHolders;
	for (const auto& title: allTitles)
	{
		if (title.second->getHolder().first && (title.second->getName().find("c_") == 0 || title.second->getName().find("b_") == 0))
		{
			countyHolders.insert(title.second->getHolder().first);
		}
		allTitleHolders[title.second->getHolder().first].insert(title);
	}

	// Then look at all potential indeps and see if their holders are up there.
	auto counter = 0;
	for (const auto& indep: potentialIndeps)
	{
		const auto& holderID = indep.second->getHolder().first;
		if (countyHolders.count(holderID))
		{
			// this fellow holds a county, so his indep title is an actual title.
			independentTitles.insert(std::pair(indep.first, indep.second));
			counter++;
			// Set The Pope(s)
			if (indep.first == "k_papal_state")
			{
				indep.second->setThePope();
				Log(LogLevel::Debug) << indep.first << " is the Pope.";
			}
			else if (indep.first == "d_fraticelli")
			{
				indep.second->setTheFraticelliPope();
				Log(LogLevel::Debug) << indep.first << " is the Fraticelli Pope.";
			}
			else
			{
				for (const auto& ownedTitle: allTitleHolders[holderID])
				{
					if (ownedTitle.first == "k_papal_state")
					{
						indep.second->setThePope();
						Log(LogLevel::Debug) << indep.first << " belongs to the Pope.";
					}
					else if (ownedTitle.first == "d_fraticelli")
					{
						indep.second->setTheFraticelliPope();
						Log(LogLevel::Debug) << indep.first << " belongs to the Fraticelli Pope.";
					}
				}
			}
		}
	}
	Log(LogLevel::Info) << "<> " << counter << " independent titles recognized.";
}

void CK2::World::mergeIndependentBaronies() const
{
	auto counter = 0;
	const auto& allTitles = titles.getTitles();
	for (const auto& title: allTitles)
	{
		const auto& holder = title.second->getHolder();
		if (!holder.first)
			continue; // don't bother with titles without holders.
		const auto& liege = title.second->getLiege();
		if (liege.first.empty())
		{
			// this is an indep.
			if (title.first.find("b_") == 0)
			{
				// it's a barony.
				const auto& djLiege = title.second->getDeJureLiege();
				if (djLiege.first.find("c_") == 0)
				{
					// we're golden.
					if (!title.second)
						Log(LogLevel::Error) << "Where is title link for " << title.first << "?";
					else
					{
						title.second->overrideLiege();
						counter++;
					}
				}
			}
		}
	}
	Log(LogLevel::Info) << "<> " << counter << " baronies reassigned.";
}

void CK2::World::congregateProvinces()
{
	auto counter = 0;
	// We're linking all contained province for a title's tree under that title.
	// This will form actual EU4 tag and contained provinces.
	for (const auto& title: independentTitles)
	{
		title.second->congregateProvinces(independentTitles);
		for (const auto& province: title.second->getProvinces())
		{
			province.second->loadHoldingTitle(std::pair(title.first, title.second));
		}
		counter += static_cast<int>(title.second->getProvinces().size());
	}
	Log(LogLevel::Info) << "<> " << counter << " provinces held by independents.";
}

void CK2::World::congregateDeJureProvinces()
{
	auto counter = 0;
	// We're linking all dejure provinces under the title as these will be the base
	// for that title's permanent claims, unless already owned.
	for (const auto& title: independentTitles)
	{
		title.second->congregateDeJureProvinces();
		counter += static_cast<int>(title.second->getDeJureProvinces().size());
	}
	Log(LogLevel::Info) << "<> " << counter << " de jure provinces claimed by independents.";
}

void CK2::World::sanityCheckifyProvinces()
{
	// This is a watchdog function intended to complain if multiple independent titles
	// link to a single province.
	std::map<int, std::vector<std::string>> provinceTitlesMap; // we store all holders for every province.
	auto sanity = true;

	for (const auto& indep: independentTitles)
	{
		const auto& ownedProvinces = indep.second->getProvinces();
		for (const auto& province: ownedProvinces)
		{
			provinceTitlesMap[province.first].push_back(indep.first);
		}
	}
	// and now, explode.
	for (const auto& entry: provinceTitlesMap)
	{
		if (entry.second.size() > 1)
		{
			std::string warning = "Province ID: " + std::to_string(entry.first) + " is owned by: ";
			for (const auto& owner: entry.second)
			{
				warning += owner + ",";
			}
			Log(LogLevel::Warning) << warning;
			sanity = false;
		}
	}
	if (sanity)
		Log(LogLevel::Info) << "<> Province sanity check passed, all provinces accounted for.";
	if (!sanity)
		Log(LogLevel::Warning) << "!! Province sanity check failed! We have excess provinces!";
}

void CK2::World::shatterEmpires(const Configuration& theConfiguration) const
{
	if (theConfiguration.getShatterEmpires() == Configuration::SHATTER_EMPIRES::NONE)
	{
		Log(LogLevel::Info) << ">< Empire shattering disabled by configuration.";
		return;
	}

	bool shatterKingdoms = true; // default
	switch (theConfiguration.getShatterLevel())
	{
		case Configuration::SHATTER_LEVEL::KINGDOM:
			shatterKingdoms = false;
			break;
		case Configuration::SHATTER_LEVEL::DUTCHY:
			shatterKingdoms = true;
			break;
	}
	const auto& allTitles = titles.getTitles();

	for (const auto& empire: allTitles)
	{
		if (hreTitle && empire.first == hreTitle->first)
			continue; // This is HRE, wrong function for that one.
		if (theConfiguration.getShatterEmpires() == Configuration::SHATTER_EMPIRES::CUSTOM && !shatterEmpiresMapper.isEmpireShatterable(empire.first))
			continue; // Only considering those listed.
		if (empire.first.find("e_") != 0 && theConfiguration.getShatterEmpires() != Configuration::SHATTER_EMPIRES::CUSTOM)
			continue; // Otherwise only empires.
		if (empire.second->getVassals().empty())
			continue; // Not relevant.

		// First we are composing a list of all members.
		std::map<std::string, std::shared_ptr<Title>> members;
		for (const auto& vassal: empire.second->getVassals())
		{
			if (vassal.first.find("d_") == 0 || vassal.first.find("c_") == 0)
			{
				members.insert(std::pair(vassal.first, vassal.second));
			}
			else if (vassal.first.find("k_") == 0)
			{
				if (shatterKingdoms && !vassal.second->isThePope() && !vassal.second->isTheFraticelliPope() && vassal.first != "k_orthodox")
				{ // hard override for special empire members
					for (const auto& vassalvassal: vassal.second->getVassals())
					{
						members.insert(std::pair(vassalvassal.first, vassalvassal.second));
					}
					// Bricking the kingdom
					vassal.second->clearVassals();
					vassal.second->clearHolder();
					vassal.second->clearLiege();
				}
				else
				{
					// Not shattering kingdoms.
					members.insert(std::pair(vassal.first, vassal.second));
				}
			}
			else if (vassal.first.find("b_") != 0)
			{
				Log(LogLevel::Warning) << "Unrecognized vassal level: " << vassal.first;
			}
		}

		for (const auto& member: members)
		{
			member.second->clearLiege();
		}

		// Finally we are clearing empire's vassal links, leaving it standalone.
		empire.second->clearVassals();
		empire.second->clearHolder();
		Log(LogLevel::Info) << "<> " << empire.first << " shattered, " << members.size() << " members released.";
	}
}

void CK2::World::flagHREProvinces(const Configuration& theConfiguration)
{
	std::string hreTitleStr;

	switch (theConfiguration.getHRE())
	{
		case Configuration::I_AM_HRE::HRE:
			hreTitleStr = "e_hre";
			break;
		case Configuration::I_AM_HRE::BYZANTIUM:
			hreTitleStr = "e_byzantium";
			break;
		case Configuration::I_AM_HRE::ROME:
			hreTitleStr = "e_roman_empire";
			break;
		case Configuration::I_AM_HRE::CUSTOM:
			hreTitleStr = iAmHreMapper.getHRE();
			break;
		case Configuration::I_AM_HRE::NONE:
			Log(LogLevel::Info) << ">< HRE Provinces not available due to configuration disabling HRE Mechanics.";
			return;
	}
	const auto& allTitles = titles.getTitles();
	const auto& theHre = allTitles.find(hreTitleStr);
	if (theHre == allTitles.end())
	{
		Log(LogLevel::Info) << ">< HRE Provinces not available, " << hreTitleStr << " not found!";
		return;
	}
	if (theHre->second->getVassals().empty())
	{
		Log(LogLevel::Info) << ">< HRE Provinces not available, " << hreTitleStr << " has no vassals!";
		return;
	}
	if (!theHre->second->getHolder().first)
	{
		Log(LogLevel::Info) << ">< HRE Provinces not available, " << hreTitleStr << " has no holder!";
		return;
	}
	// store for later.
	hreTitle = std::make_pair(hreTitleStr, theHre->second);

	const auto counter = theHre->second->flagDeJureHREProvinces();
	Log(LogLevel::Info) << "<> " << counter << " HRE provinces flagged.";
}

void CK2::World::shatterHRE(const Configuration& theConfiguration) const
{
	if (!hreTitle)
		return;

	const auto& hreHolder = hreTitle->second->getHolder();
	bool emperorSet = false;

	// First we are composing a list of all HRE members. These are duchies,
	// so we're also ripping them from under any potential kingdoms.
	std::map<std::string, std::shared_ptr<Title>> hreMembers;
	for (const auto& vassal: hreTitle->second->getVassals())
	{
		if (vassal.first.find("d_") == 0 || vassal.first.find("c_") == 0)
		{
			hreMembers.insert(std::pair(vassal.first, vassal.second));
		}
		else if (vassal.first.find("k_") == 0)
		{
			if (vassal.first == "k_papacy" || vassal.first == "k_papal_state" || vassal.first == "k_orthodox" ||
				 theConfiguration.getShatterHRELevel() == Configuration::SHATTER_HRE_LEVEL::KINGDOM) // hard override for special HRE members
			{
				hreMembers.insert(std::pair(vassal.first, vassal.second));
				continue;
			}
			for (const auto& vassalvassal: vassal.second->getVassals())
			{
				hreMembers.insert(std::pair(vassalvassal.first, vassalvassal.second));
			}
			// Bricking the kingdom.
			vassal.second->clearVassals();
			vassal.second->clearHolder();
			vassal.second->clearLiege();
		}
		else if (vassal.first.find("b_") != 0)
		{
			Log(LogLevel::Warning) << "Unrecognized HRE vassal: " << vassal.first;
		}
	}

	// Locating HRE emperor. Starting by kingdoms downwards, trying to see which of his titles holds the capital.
	const auto& hreCapital = hreHolder.second->getCapitalProvince();
	if (hreCapital.first)
	{
		Log(LogLevel::Debug) << "HRE Capital: " << hreCapital.first << " - " << hreCapital.second->getName();
		const auto& nominalKingdom = hreCapital.second->belongsToKingdom();
		if (nominalKingdom)
		{
			Log(LogLevel::Debug) << "Nominal kingdom: " << nominalKingdom->first;
			for (const auto& member: hreMembers)
			{
				if (member.first.find("k_") != 0)
					continue;
				if (nominalKingdom->first == member.first && member.second->getHolder().first == hreHolder.first)
				{
					member.second->setHREEmperor();
					emperorSet = true;
					hreHolder.second->overridePrimaryTitle(member);
					Log(LogLevel::Debug) << "HRE Emperor set via capital: " << member.first;
					break;
				}
			}
		}
		if (!emperorSet)
		{
			const auto& nominalDuchy = hreCapital.second->belongsToDuchy();
			if (nominalDuchy)
			{
				Log(LogLevel::Debug) << "Nominal duchy: " << nominalDuchy->first;
				for (const auto& member: hreMembers)
				{
					if (member.first.find("d_") != 0)
						continue;
					if (nominalDuchy->first == member.first && member.second->getHolder().first == hreHolder.first)
					{
						member.second->setHREEmperor();
						emperorSet = true;
						hreHolder.second->overridePrimaryTitle(member);
						Log(LogLevel::Debug) << "HRE Emperor set via capital: " << member.first;
						break;
					}
				}
			}
		}
		if (!emperorSet)
		{
			Log(LogLevel::Debug) << "HRE capital's county: " << hreCapital.second->getDeJureTitle().first;
			for (const auto& member: hreMembers)
			{
				if (member.first.find("c_") != 0)
					continue;
				if (hreCapital.second->getDeJureTitle().first == member.first && member.second->getHolder().first == hreHolder.first)
				{
					member.second->setHREEmperor();
					emperorSet = true;
					hreHolder.second->overridePrimaryTitle(member);
					Log(LogLevel::Debug) << "HRE Emperor set via capital: " << member.first;
					break;
				}
			}
		}
	}
	else
	{
		Log(LogLevel::Warning) << "HRE Emperor has no capital? Disturbing. Will pick something at random.";
	}

	if (!emperorSet)
		Log(LogLevel::Debug) << "Couldn't find His HREship by capital province. Picking first owned non-barony.";

	for (const auto& member: hreMembers)
	{
		// We're flagging hre members as such, as well as setting them free.
		// We're also on the lookout on the current HRE emperor if we're missing one.
		if (!emperorSet && member.second->getHolder().first == hreHolder.first && member.first.find("b_") == std::string::npos)
		{
			// This is the emperor. He may hold several duchies, but the first one
			// we find will be flagged emperor.
			member.second->setHREEmperor();
			emperorSet = true;
			hreHolder.second->overridePrimaryTitle(member);
			Log(LogLevel::Debug) << "Emperor is " << member.first;
		}
		member.second->setInHRE();
		member.second->clearLiege();
	}

	// Finally we are clearing hreTitle's vassal links, leaving it standalone.
	hreTitle->second->clearVassals();
	hreTitle->second->clearHolder();
	Log(LogLevel::Info) << "<> " << hreMembers.size() << " HRE members released.";
}

void CK2::World::filterProvincelessTitles()
{
	auto counter = 0;
	std::set<std::string> titlesForDisposal;
	for (const auto& title: independentTitles)
	{
		if (title.second->getProvinces().empty())
		{
			titlesForDisposal.insert(title.first);
		}
	}
	for (const auto& drop: titlesForDisposal)
	{
		independentTitles.erase(drop);
		counter++;
	}
	Log(LogLevel::Info) << "<> " << counter << " empty titles dropped, " << independentTitles.size() << " remain.";
}

void CK2::World::createReformedFeatures()
{

	// Unique Reforms (Only the unique reforms that have unique modifiers such as "uses_karma")
	std::set<std::string> unique = {"religion_feature_norse",
		 "religion_feature_tengri",
		 "religion_feature_west_african",
		 "religion_feature_bon",
		 "religion_feature_hellenic"};

	if (!reformationList.empty())
	{
		wereNoReformations = false;
	}

	// All Reformed Religions
	for (const auto& reformation: reformationList)
	{
		mappers::ReformedReligionMapping tempReligion;

		tempReligion.setName(reformation);
		tempReligion.setIconNumber(reformedReligionMapper.getReligionEntries().find(reformation)->second.getIconNumber());
		tempReligion.setColor(reformedReligionMapper.getReligionEntries().find(reformation)->second.getColor());
		for (auto tempReform: religions.getReformedReligion().find(reformation)->second)
		{
			tempReligion.addCountryModifiers(reformedReligionMapper.getReligionEntries().find(tempReform)->second.getCountryModifiers());
			tempReligion.addProvinceModifiers(reformedReligionMapper.getReligionEntries().find(tempReform)->second.getProvinceModifiers());
			tempReligion.addSecondary(reformedReligionMapper.getReligionEntries().find(tempReform)->second.getSecondary());
			if (unique.count(tempReform) || tempReligion.getUniqueMechanics().length() == 0) // Ensures that unique Mechanics get in
				tempReligion.setUniqueMechanics(reformedReligionMapper.getReligionEntries().find(tempReform)->second.getUniqueMechanics());
			tempReligion.addNonUniqueMechanics(reformedReligionMapper.getReligionEntries().find(tempReform)->second.getNonUniqueMechanics());
		}
		tempReligion.setHereticStrings(reformedReligionMapper.getReligionEntries().find(reformation)->second.getHereticStrings());

		religionReforms.push_back(tempReligion);
	}

	// And now for all the unreformed religions
	for (const auto& religion: reformedReligionMapper.getReligionEntries())
	{
		bool tester = false;
		for (auto reform: religionReforms)
		{
			if (religion.first == reform.getName() || !reform.getName().find("religion_") || !religion.first.find("religion_"))
			{
				tester = true;
				break;
			}
		}
		if (!tester)
			unreformationList.insert(religion.first);
	}

	for (auto unreformed: unreformationList)
	{
		mappers::ReformedReligionMapping tempUnreligion;

		tempUnreligion.setName(unreformed);
		tempUnreligion.setIconNumber(reformedReligionMapper.getReligionEntries().find(unreformed)->second.getIconNumber());
		tempUnreligion.setColor(reformedReligionMapper.getReligionEntries().find(unreformed)->second.getColor());
		tempUnreligion.setCountryModifiers(reformedReligionMapper.getReligionEntries().find(unreformed)->second.getCountryModifiers());
		tempUnreligion.setProvinceModifiers(reformedReligionMapper.getReligionEntries().find(unreformed)->second.getProvinceModifiers());
		tempUnreligion.setSecondary(reformedReligionMapper.getReligionEntries().find(unreformed)->second.getSecondary());
		tempUnreligion.setUniqueMechanics(reformedReligionMapper.getReligionEntries().find(unreformed)->second.getUniqueMechanics());
		tempUnreligion.setNonUniqueMechanics(reformedReligionMapper.getReligionEntries().find(unreformed)->second.getNonUniqueMechanics());
		tempUnreligion.setHereticStrings(reformedReligionMapper.getReligionEntries().find(unreformed)->second.getHereticStrings());

		unreligionReforms.push_back(tempUnreligion);
	}

	Log(LogLevel::Info) << "<> " << reformationList.size() << " religion(s) reformed in CK2.";
}
