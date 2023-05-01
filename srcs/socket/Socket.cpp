#include "../../incs/socket/Socket.hpp"
#include <new>
#include <sys/_endian.h>

// Constructor & Destructor
Socket::Socket(void) {}
Socket::Socket(int port) {
	if ((this->_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		throw (Socket::FailToCreateException());
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
bool	Socket::bind(void) throw(std::exception) {
	if (::bind(this->_fd, (struct sockaddr *)&this->_addr, sizeof(this->_addr)) == -1) {
		throw (Socket::FailToBindException());
	}
	return (true);
}

bool	Socket::listen(void) throw(std::exception) {
	if (::listen(this->_fd, 5) == -1) {
		throw (Socket::FailToListenException());
	}
	return (true);
}

int	Socket::accept(void) throw(std::exception) {
	int					client_fd;
	struct sockaddr_in	client_addr;
	socklen_t			client_addr_len;

	client_addr_len = sizeof(client_addr);
	if ((client_fd = ::accept(this->_fd, (struct sockaddr *)&client_addr, &client_addr_len)) == -1) {
		throw (Socket::FailToAcceptException());
	}
	return (client_fd);
}

// Exception
const char	*Socket::FailToCreateException::what(void) const throw() { return ("Fail to create socket"); }
const char	*Socket::FailToBindException::what(void) const throw() { return ("Fail to bind socket"); }
const char	*Socket::FailToListenException::what(void) const throw() { return ("Fail to listen socket"); }
const char	*Socket::FailToAcceptException::what(void) const throw() { return ("Fail to accept socket"); }
