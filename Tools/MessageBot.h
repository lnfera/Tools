#pragma once
#include <string>
#include <Windows.h>
#include <fstream>
#include <vector>
#include <filesystem>
namespace Tools
{
	class MessageBot
	{
	public:
		MessageBot(std::string aWebhookURL, bool aAutoSend) :myWebhookUrl(aWebhookURL), myAutoSend(aAutoSend) 
		{
		};

		void SendMessageToDiscord(const std::string aMessage)
		{
			const std::string cmd_1 = "curl -i -H \"Accept: application/json\" -H \"Content-Type:application/json\" -X POST --data \"{\\\"content\\\": \\\"";
			const std::string cmd_2 = "\\\"}\" ";
			std::string command = cmd_1 + aMessage + cmd_2 + myWebhookUrl;

			bool sendMsg = false;
			if (myAutoSend)
			{
				sendMsg = true;
			}
			else
			{
				if (DoYouWantToSendCheck())	// Runs windows message box
				{
					sendMsg = true;
				}
			}

			if (sendMsg)
			{
				SystemNoOutput(command);	// sends the message
			}
		}
		void SendFileToDiscord(const std::string aFilePath)
		{
			const std::string cmd_1 = "curl -F \"file1=@";
			const std::string cmd_2 = "\" ";
			const std::string image_Url = aFilePath;

			std::string command = cmd_1 + aFilePath + cmd_2 + myWebhookUrl;
			system(command.c_str());
		}
	private:
		int SystemNoOutput(std::string aCommand)
		{
			aCommand.insert(0, "/C ");

			SHELLEXECUTEINFOA ShExecInfo = { 0 };
			ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
			ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
			ShExecInfo.hwnd = NULL;
			ShExecInfo.lpVerb = NULL;
			ShExecInfo.lpFile = "cmd.exe";
			ShExecInfo.lpParameters = aCommand.c_str();
			ShExecInfo.lpDirectory = NULL;
			ShExecInfo.nShow = SW_HIDE;
			ShExecInfo.hInstApp = NULL;

			if (ShellExecuteExA(&ShExecInfo) == FALSE)
				return -1;

			WaitForSingleObject(ShExecInfo.hProcess, INFINITE);

			DWORD rv;
			GetExitCodeProcess(ShExecInfo.hProcess, &rv);
			CloseHandle(ShExecInfo.hProcess);

			return rv;
		}
		bool DoYouWantToSendCheck()
		{
			return MessageBox(
				NULL,
				L"Would you like to send a message?",
				L"Message Bot",
				MB_YESNO | MB_ICONQUESTION | MB_TOPMOST
			) == IDYES;
		}
	
		std::string myWebhookUrl = "undefined";
		bool myAutoSend = false;
	};
}