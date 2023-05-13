#include "../../incs/socket/Socket.hpp"
#include <new>
#include <sys/_endian.h>
#include <memory.h>

// Constructor & Destructor
Socket::Socket(void): _fd(0), _enable(0) { memset(&this->_addr, 0, sizeof(struct sockaddr_in)); }
Socket::Socket(int port): _fd(0), _enable(0) {
	// Create socket
	if ((this->_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		throw (FailToCreateException());
	}

	// Set socket options
	this->_enable = 1;
	setsockopt(this->_fd, SOL_SOCKET, SO_REUSEADDR, &this->_enable, sizeof(this->_enable));
	memset(&this->_addr, 0, sizeof(this->_addr));
	this->_addr.sin_family = AF_INET;
	this->_addr.sin_port = htons(port);
	this->_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	// Bind & Listen
	if (bind(this->_fd, (struct sockaddr *)&this->_addr, sizeof(this->_addr)) == -1) {
		throw (FailToBindException());
	}
	if (listen(this->_fd, 10) == -1) {
		throw (FailToListenException());
	}
}
Socket::Socket(const Socket &ref): _fd(ref._fd), _enable(ref._enable), _addr(ref._addr) {}
Socket::~Socket(void) {}
Socket	&Socket::operator=(const Socket &rhs) {
	if (this != &rhs) {
		this->~Socket();
		new (this) Socket(rhs);
	}
	return (*this);
}

// Utils
int	Socket::getFd(void) const { return (this->_fd); }

// Exception
const char	*Socket::FailToCreateException::what(void) const throw() { return ("Socket: Fail to create"); }
const char	*Socket::FailToBindException::what(void) const throw() { return ("Socket: Fail to bind"); }
const char	*Socket::FailToListenException::what(void) const throw() { return ("Socket: Fail to listen"); }