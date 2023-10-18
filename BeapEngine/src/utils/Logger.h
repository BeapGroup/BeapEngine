#pragma once
#include <iostream>

namespace beap
{
	namespace utils
	{

		enum ELogErrorSeverity
		{
			WARNING,
			ERROR,
			FATAL
		};

		class Logger
		{
		public:
			static void Log(const char* Log) 
			{
				std::cout << "[BEAPENGINE | LOG] " << Log << std::endl;
			}

			// TODO: Colour log by severity level.
			static void LogError(const char* Log, ELogErrorSeverity severity)
			{
				std::cout << "[BEAPENGINE | " << ERROR << "] " << Log << std::endl;
			}
		};
	}
}