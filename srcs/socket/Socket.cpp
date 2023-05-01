#include "../../incs/socket/Socket.hpp"
#include <new>
#include <sys/_endian.h>

// Constructor & Destructor
Socket::Socket(void) {}
Socket::Socket(int port) {
	if ((this->_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		throw (Socket::SocketCreationErrorException());
	}
	this->_addr.sin_family = AF_INET;
	this->_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	this->_addr.sin_port = htons(port);
}
Socket::~Socket(void) {}
Socket::Socket(const Socket &ref): _fd(ref._fd), _addr(ref._addr) {}
Socket	&Socket::operator=(const Socket &rhs) {
	if (this != &rhs) {
		this->~Socket();
		new (this) Socket(rhs);
	}
	return (*this);
}

// Getter
int		Socket::getFd(void) const { return (this->_fd); }

// Utils
bool	Socket::bind(void) {
	if (::bind(this->_fd, (struct sockaddr *)&this->_addr, sizeof(this->_addr)) == -1) {
		throw (Socket::SocketBindErrorException());
	}
	return (true);
}

bool	Socket::listen(void) {
	if (::listen(this->_fd, 5) == -1) {
		throw (Socket::SocketListenErrorException());
	}
	return (true);
}

// Exception
const char	*Socket::SocketCreationErrorException::what(void) const throw() { return ("Socket creation error"); }
const char	*Socket::SocketBindErrorException::what(void) const throw() { return ("Socket bind error"); }
const char	*Socket::SocketListenErrorException::what(void) const throw() { return ("Socket listen error"); }
