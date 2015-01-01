#ifndef GETAPPDATAPATH_H__
#define GETAPPDATAPATH_H__

#include <string>
#include <vector>
#include <stdexcept>

#ifdef _WIN32
#include <ShlObj.h>

// Beware, including ShlObj.h causes lot of global namespace pollution,
// you may need to clean it up, for example:
//
// #if defined(_WIN32) && defined(ERROR)
// #undef ERROR
// #endif

namespace logcpp
{

std::string GetAppDataPath(const std::string& appFolder,
                           const std::string& fileName)
{
    std::vector<char> buf(MAX_PATH);
    if (SHGetSpecialFolderPathA(nullptr, &buf[0], CSIDL_LOCAL_APPDATA, FALSE) !=
        TRUE) {
        throw std::runtime_error("SHGetSpecialFolderPathA failed");
    }
    auto path = std::string(&buf[0]);

    path.push_back('\\');
    path.append(appFolder);
    // TODO: look up API docs if < 0 is sane
    if (SHCreateDirectoryExA(nullptr, path.c_str(), nullptr) < 0) {
        throw std::runtime_error("SHCreateDirectoryExA failed");
    }

    path.push_back('\\');
    path.append(fileName);

    return path;
}

// namespace
}

#else
#error "GetAppDataPath not yet implemented in *nix"
#endif

#endif /* GETAPPDATAPATH_H */
