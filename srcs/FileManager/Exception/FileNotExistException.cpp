#include <FileManager/Exception/FileNotExistException.hpp>

FileNotExistException::FileNotExistException(const std::string &msg) : _msg(msg) {}
FileNotExistException::~FileNotExistException() throw() {}
const char *FileNotExistException::what() const throw() {
    _fullMsg = "FileNotExistException: " + _msg;  // 이렇게 하면 메모리 할당이 발생할 수 있음
    return _fullMsg.c_str();
}
