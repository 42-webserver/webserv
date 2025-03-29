#include "../../incs/Channel/FileStream.hpp"
#include <FileManager/FileManager/FileManager.hpp>

void FileStream::inJectChannelFd(int fd) {
    this->setChannelFd(fd);
}

bool FileStream::ensure_directory_exists(const std::string& path) {
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

FILE* FileStream::fopen_with_dirs(const std::string& path, const char* mode) {
    std::string filePath = path;
    // 수정: C++98에서는 pop_back() 대신 resize()로 마지막 문자를 제거
    if (filePath[filePath.length() - 1] == '/') {
        filePath.resize(filePath.size() - 1);
    }
    std::string directoryPath = filePath.substr(0, filePath.find_last_of('/'));
    if (!FileStream::ensure_directory_exists(directoryPath)) {
        return NULL;
    }
    return fopen(filePath.c_str(), mode);
}


FileStream::FileStream(std::string path, std::string mode) {
    if (mode == "w" || mode == "w+" || mode == "wb" || mode == "w+b"
        || mode == "a" || mode == "a+" || mode == "ab" || mode == "a+b") {
            this->_fp = fopen_with_dirs(path, mode.c_str());
        if (this->_fp == NULL) {
            //log error
            throw std::runtime_error(std::string("FileStream: Fail to open ") + path);
        }
        this->inJectChannelFd(fileno(this->_fp));
        return ;
    }
    else if (mode == "r" || mode == "r+" || mode == "rb" || mode == "r+b") {
            this->_fp = fopen(path.c_str(), mode.c_str()); // 이미 이진 모드로 지정됨
        if (this->_fp == NULL) {
            //log error
            throw NotFoundException();
        }
        this->inJectChannelFd(fileno(this->_fp));
    }
    else {
        throw std::invalid_argument("FileStream: invalid mode");
    }
}


FileStream::~FileStream(void) {
    if (this->_fp != NULL)
        fclose(this->_fp);
}

const char	*FileStream::FailToOpenException::what(void) const throw() { return ("FileStream: Fail to open"); }