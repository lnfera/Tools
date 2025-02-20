#include "ConsoleSystem.h"
#include <chrono>

constexpr int MAX_LOG_SIZE = 100;
static std::vector<Tga::Console::LogEntry> globalLogs;
static Tga::Color globalColors[Tga::Console::Colors::COUNT] =
{
	{1.0f,1.0f,1.0f,1.0f},	//White
	{1.0f,0.0f,0.0f,1.0f},	//Red
	{0.0f,1.0f,0.0f,1.0f},	//Green
	{0.2f,0.2f,1.0f,1.0f},	//Blue
	{0.5f,0.2f,0.8f,1.0f},	//Magenta
	{0.8f,0.8f,0.0f,1.0f},	//Yellow
};


void Tga::Console::LogMessage(std::string aMessage, Colors aColor)
{
	LogMessage(aMessage, globalColors[aColor]);
}
void Tga::Console::LogMessage(std::string aMessage, Tga::Color aColor)
{
	std::string time;
	{
		std::time_t now = std::time(nullptr);
#pragma warning(suppress : 4996)
		std::tm* local_time = std::localtime(&now);

		std::string t[3] = {
			std::to_string(local_time->tm_hour),
			std::to_string(local_time->tm_min),
			std::to_string(local_time->tm_sec)
		};

		for (int i = 0; i < 3; i++)
		{
			if (t[i].size() < 2)
			{
				t[i] = "0" + t[i];
			}

		}

		time = t[0] + ":" + t[1] + ":" + t[2];
	}


	LogEntry entry;
	entry.message = aMessage;
	entry.color = aColor;
	entry.time = time;

	globalLogs.insert(globalLogs.begin(), entry);

	if ((int)globalLogs.size() >= MAX_LOG_SIZE)
	{
		globalLogs.pop_back();
	}

}

void Tga::Console::ClearLogs()
{
	globalLogs.clear();

	std::string time;
	{
		std::time_t now = std::time(nullptr);
#pragma warning(suppress: 4996)
		std::tm* local_time = std::localtime(&now);

		std::string t[3] = {
			std::to_string(local_time->tm_hour),
			std::to_string(local_time->tm_min),
			std::to_string(local_time->tm_sec)
		};

		for (int i = 0; i < 3; i++)
		{
			if (t->size() < 2)
			{
				t[i] = "0" + t[i];
			}

		}

		time = t[0] + ":" + t[1] + ":" + t[2];
	}


	LogEntry entry;
	entry.message = "CLEARED CONSOLE";
	entry.color = globalColors[Tga::Console::Colors::White];
	entry.time = time;

	globalLogs.insert(globalLogs.begin(), entry);
}

std::vector<Tga::Console::LogEntry> Tga::Console::GetLogs()
{
	return globalLogs;
}
