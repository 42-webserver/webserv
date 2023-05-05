#include "../../incs/socket/Socket.hpp"
#include <new>
#include <sys/_endian.h>
#include <unistd.h>
#include <fcntl.h>
#include <memory.h>

// Constructor & Destructor
Socket::Socket(void) {}
Socket::Socket(int port) {
	if ((this->_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		throw (Socket::FailToCreateException());
	}
	this->_enable = 1;
	setsockopt(this->_fd, SOL_SOCKET, SO_REUSEADDR, &this->_enable, sizeof(this->_enable));
	memset(&this->_addr, 0, sizeof(this->_addr));
	this->_addr.sin_family = AF_INET;
	this->_addr.sin_port = htons(port);
	this->_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	try {
		this->bind();
		this->listen();
	} catch (const Socket::FailToBindException &e) {
		close(this->_fd);
		throw (e);
	} catch (const Socket::FailToListenException &e) {
		close(this->_fd);
		throw (e);
	}
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
	if (::bind(this->_fd, (struct sockaddr *) &this->_addr, sizeof(this->_addr)) == -1) {
		throw (Socket::FailToBindException());
	}
	return (true);
}

bool	Socket::listen(void) throw(std::exception) {
	if (::listen(this->_fd, 10) == -1) {
		throw (Socket::FailToListenException());
	}
	return (true);
}

int	Socket::accept(void) throw(std::exception) {
	int					client_fd;
	struct sockaddr_in	client_addr;
	socklen_t			client_addr_len;
	int					flags;

	client_addr_len = sizeof(client_addr);
	if ((client_fd = ::accept(this->_fd, (struct sockaddr *)&client_addr, &client_addr_len)) == -1) {
		throw (Socket::FailToAcceptException());
	}
	if ((flags = fcntl(client_fd, F_GETFL, 0)) == -1) {
		close(client_fd);
		throw (Socket::FailToControlException());
	}
	if (fcntl(client_fd, F_SETFL, flags | O_NONBLOCK) == -1) {
		close(client_fd);
		throw (Socket::FailToControlException());
	}
	return (client_fd);
}

// Exception
const char	*Socket::FailToCreateException::what(void) const throw() { return ("Socket: Fail to create"); }
const char	*Socket::FailToBindException::what(void) const throw() { return ("Socket: Fail to bind"); }
const char	*Socket::FailToListenException::what(void) const throw() { return ("Socket: Fail to listen"); }
const char	*Socket::FailToAcceptException::what(void) const throw() { return ("Socket: Fail to accept"); }
const char	*Socket::FailToControlException::what(void) const throw() { return ("Socket: Fail to control"); }