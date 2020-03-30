#include "Log.h"
#include "CK2ToEU4Converter.h"

int main(const int argc, const char * argv[])
{
	try
	{
		const mappers::VersionParser versionParser;
		LOG(LogLevel::Info) << versionParser;
		LOG(LogLevel::Info) << "Built on " << __TIMESTAMP__;
		if (argc >= 2)
		{
			LOG(LogLevel::Info) << "CK2ToEU4 takes no parameters.";
			LOG(LogLevel::Info) << "It uses configuration.txt, configured manually or by the frontend.";
		}
		convertCK2ToEU4(versionParser);
		return 0;
	}

	catch (const std::exception& e)
	{
		LOG(LogLevel::Error) << e.what();
		return -1;
	}
}
