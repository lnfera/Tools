#pragma once
#include <Windows.h>
#include <string>
namespace Tools
{

    LPWSTR ConvertToLPWSTR(const char* aString)
    {
        // Step 1: Get the length of the input string
        int len = strlen(aString);

        // Step 2: Calculate the required buffer size for wide characters
        int bufferSize = MultiByteToWideChar(CP_UTF8, 0, aString, len, NULL, 0);

        // Step 3: Allocate memory for the wide string
        LPWSTR wideStr = new wchar_t[bufferSize + 1]; // +1 for null terminator

        // Step 4: Perform the conversion
        MultiByteToWideChar(CP_UTF8, 0, aString, len, wideStr, bufferSize);

        // Step 5: Null-terminate the wide string
        wideStr[bufferSize] = L'\0';

        return wideStr;
    }

    bool SetWallpaper(const char* aWallpaperPath) 
    {
        LPWSTR lpwstr = ConvertToLPWSTR(aWallpaperPath);
        return SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, lpwstr, SPIF_UPDATEINIFILE);
    }

    bool OpenLink(const char* aURLPath)
    {
        return ShellExecuteW(0, 0, ConvertToLPWSTR(aURLPath), 0, 0, SW_HIDE);
    }

    void ShutdownComputer() {
        system("shutdown /s /t 0");
    }
}