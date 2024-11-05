#pragma once
#include <filesystem>
#include <string>
namespace Tools
{
    std::filesystem::path GetAbsolutePath(const std::string& aRelativePath) {
        // Convert relative path to absolute path
        std::filesystem::path absPath = std::filesystem::absolute(aRelativePath);
        return absPath;
    }
}