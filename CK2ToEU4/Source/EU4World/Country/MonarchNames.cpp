#include "MonarchNames.h"
#include "Log.h"

EU4::MonarchNames::MonarchNames(std::istream& theStream)
{
	// This is a very special manual parsing class that we have to use to load monarch names.
	// They come in form: "Mohammed #0" = 100
	// Parser will choke on quoted key so we'll do this manually.

	std::string line;
	while (std::getline(theStream, line))
	{
		if (line.find('{') != std::string::npos)
			continue;
		if (line.find('}') != std::string::npos)
			return;
		if (line.length() < 10)
			continue;
		if (line.find('#') == 0)
			continue;

		std::string name;
		auto regnal = 0;
		auto chance = 0;

		auto quoteLoc = line.find_first_of('\"');
		if (quoteLoc == std::string::npos)
			continue;
		auto newline = line.substr(quoteLoc + 1, line.length());
		quoteLoc = newline.find_last_of('\"');
		if (quoteLoc == std::string::npos)
			continue;
		auto fullname = newline.substr(0, quoteLoc);
		if (fullname.find('#') != std::string::npos)
		{
			const auto hashpos = fullname.find('#');
			name = fullname.substr(0, hashpos - 1);
			regnal = std::stoi(fullname.substr(hashpos + 1, fullname.length()));
		}
		else
		{
			name = fullname;
		}

		const auto eqPos = line.find('=');
		newline = line.substr(eqPos + 1, line.length());
		if (newline.find('#') != std::string::npos)
		{
			const auto hashpos = newline.find('#');
			newline = newline.substr(0, hashpos);
		}
		chance = std::stoi(newline);

		monarchNames.insert(std::pair(name, std::pair(regnal, chance)));
	}
}
