#include "../../incs/FileDescriptor/FileDescriptor.hpp"

FileDescriptor::FileDescriptor(void) : _fd(0) {}
FileDescriptor::FileDescriptor(int fd) : _fd(fd) {}
FileDescriptor::~FileDescriptor(void) { ::close(this->_fd);};
FileDescriptor::FileDescriptor(const FileDescriptor &ref) : _fd(ref._fd) {}
FileDescriptor	&FileDescriptor::operator=(const FileDescriptor &rhs) {
	if (this != &rhs) {
		this->~FileDescriptor();
		new (this) FileDescriptor(rhs);
	}
	return (*this);
}

int	FileDescriptor::getFd() const { return this->_fd; }