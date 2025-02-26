#pragma once
#include <string>
#include <vector>

#include <tge/math/color.h>
namespace Tga
{
	namespace Console
	{
		enum Colors
		{
			White = 0,
			Red,
			Green,
			Blue,
			Magenta,
			Yellow,


			COUNT
		};

		struct LogEntry
		{
			std::string message = "x";
			Tga::Color color = { 1,1,1,1 };
			std::string time = "-1";
		};


		void LogMessage(std::string aMessage, Colors aColor);
		void LogMessage(std::string aMessage, Tga::Color aColor);

		void ClearLogs();

		std::vector<LogEntry> GetLogs();
	}
}
