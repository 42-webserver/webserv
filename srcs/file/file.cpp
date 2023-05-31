#include "../../incs/file/file.hpp"

FdClass::FdClass(int fd) : _fd(fd) {}
FdClass::~FdClass() { ::close(this->_fd);};
const int &FdClass::getFd() const { return this->_fd; }