#include <iostream>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>

bool ensure_directory_exists(const std::string& path) {
    struct stat st;
    for (size_t i = 1; i < path.length(); ++i) { // start from 1 to skip the first '/'
        if (path[i] == '/') {
            std::string dir = path.substr(0, i);
            if (stat(dir.c_str(), &st) != 0 || !S_ISDIR(st.st_mode)) {
                if (mkdir(dir.c_str(), S_IRWXU) != 0) {
                    return false;
                }
            }
        }
    }
    if (path[path.length() - 1] != '/') {
        std::string dir = path;
        if (stat(dir.c_str(), &st) != 0 || !S_ISDIR(st.st_mode)) {
            if (mkdir(dir.c_str(), S_IRWXU) != 0) {
                return false;
            }
        }
    }
    return true;
}

FILE* fopen_with_dirs(const std::string& path, const char* mode) {
    std::string filePath = path;
    if (filePath[filePath.length() - 1] == '/') {
        filePath.pop_back();
    }
    std::string directoryPath = filePath.substr(0, filePath.find_last_of('/'));
    if (!ensure_directory_exists(directoryPath)) {
        return NULL;
    }
    return fopen(filePath.c_str(), mode);
}
