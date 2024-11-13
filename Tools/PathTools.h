#pragma once
#include <filesystem>
#include <string>
namespace Tools
{
    std::filesystem::path GetAbsolutePath(const std::string& aRelativePath) 
    {
        // Convert relative path to absolute path
        return std::filesystem::absolute(aRelativePath);
    }
}