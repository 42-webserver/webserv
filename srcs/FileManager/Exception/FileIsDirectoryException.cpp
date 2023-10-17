#include <FileManager/Exception/FileIsDirectoryException.hpp>

FileIsDirectoryException::FileIsDirectoryException(const std::string &msg) : _msg(msg) {}
FileIsDirectoryException::~FileIsDirectoryException() throw() {}
const char *FileIsDirectoryException::what() const throw() {
    _fullMsg = "FileIsDirectoryException: " + _msg;  // 이렇게 하면 메모리 할당이 발생할 수 있음
    return _fullMsg.c_str();
}
