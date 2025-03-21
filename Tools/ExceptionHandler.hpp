#pragma once
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <DbgHelp.h>
#include <strsafe.h>
#include <shellapi.h>
#include <shlobj.h>
#include <direct.h>
#include <cassert>

inline void CreateMiniDump(EXCEPTION_POINTERS* someExceptionPointers);

LONG WINAPI ExceptionFilterFunction(_EXCEPTION_POINTERS* aExceptionP)
{
    CreateMiniDump(aExceptionP);

    return EXCEPTION_EXECUTE_HANDLER;
}

inline void CreateMiniDump(EXCEPTION_POINTERS* aExceptionPointers)
{
    BOOL miniDumpSuccessful;
    WCHAR fileName[MAX_PATH];
    HANDLE dumpFile;
    SYSTEMTIME localTime;
    MINIDUMP_EXCEPTION_INFORMATION expParam;

    // Get current build directory
    char currentPath[MAX_PATH];
    if (!_getcwd(currentPath, MAX_PATH))
    {
        assert(false && "Failed to get current directory");
    }

    // Append "/DumpFiles/" to the build directory
    std::string dumpPath = std::string(currentPath) + "/DumpFiles/";

    // Ensure the directory exists
    if (_mkdir(dumpPath.c_str()) != 0 && errno != EEXIST)
    {
        assert(false && "Failed to create dump files");

    }

    // Convert the dumpPath to wide characters
    WCHAR dumpDir[MAX_PATH];
    MultiByteToWideChar(CP_ACP, 0, dumpPath.c_str(), -1, dumpDir, MAX_PATH);

    // Get the current local time
    GetLocalTime(&localTime);

    // Format the dump file name
    StringCchPrintf(fileName, MAX_PATH, L"%sCrashReport-%i%i%i%i%i.dmp",
        dumpDir,localTime.wYear,localTime.wMonth,localTime.wDay,localTime.wHour,localTime.wMinute);

    // Create the dump file
    dumpFile = CreateFile(fileName, GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_WRITE | FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, 0);

    if (dumpFile == INVALID_HANDLE_VALUE)
    {
        std::wcerr << L"Failed to create dump file: " << fileName << std::endl;
        return;
    }

    // Set up the exception information
    expParam.ThreadId = GetCurrentThreadId();
    expParam.ExceptionPointers = aExceptionPointers;
    expParam.ClientPointers = TRUE;

    // Write the minidump
    miniDumpSuccessful = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),
        dumpFile, MiniDumpWithDataSegs, &expParam, NULL, NULL);

    if (miniDumpSuccessful)
    {
        std::wcout << L"Minidump created successfully: " << fileName << std::endl;
    }
    else
    {
        std::wcerr << L"Failed to write minidump." << std::endl;
    }

    CloseHandle(dumpFile);

    MessageBox(nullptr, L"Tell a programmer!", L"Error has been catched!", MB_OK);

    assert(false && "ERROR HAS BEEN CATCHED");
}
