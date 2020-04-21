#include "CK2ToEU4Converter.h"
#include "Log.h"

int main(const int argc, const char* argv[])
{
	try
	{
		const mappers::VersionParser versionParser;
		Log(LogLevel::Info) << versionParser;
		if (argc >= 2)
		{
			Log(LogLevel::Info) << "CK2ToEU4 takes no parameters.";
			Log(LogLevel::Info) << "It uses configuration.txt, configured manually or by the frontend.";
		}
		convertCK2ToEU4(versionParser);
		return 0;
	}

	catch (const std::exception& e)
	{
		Log(LogLevel::Error) << e.what();
		return -1;
	}
}
