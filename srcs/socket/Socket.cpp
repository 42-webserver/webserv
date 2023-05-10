#include "../../incs/socket/Socket.hpp"
#include <new>
#include <sys/_endian.h>
#include <unistd.h>
#include <fcntl.h>
#include <memory.h>

// Constructor & Destructor
Socket::Socket(void): _fd(0) {}
Socket::Socket(int fd): _fd(fd) {}
Socket::Socket(const Socket &ref): _fd(ref._fd) {}
Socket::~Socket(void) { if (this->_fd) close(this->_fd); }
Socket	&Socket::operator=(const Socket &rhs) {
	if (this != &rhs) {
		this->~Socket();
		new (this) Socket(rhs);
	}
	return (*this);
}

// Getter
int		Socket::getFd(void) const { return (this->_fd); }

// Exception
const char	*Socket::FailToCreateException::what(void) const throw() { return ("Socket: Fail to create"); }