#include "../../incs/socket/Socket.hpp"
#include <new>
#include <sys/_endian.h>
#include <unistd.h>
#include <fcntl.h>
#include <memory.h>

// Constructor & Destructor
Socket::Socket(void): _fd(0), _enable(0) { memset(&this->_addr, 0, sizeof(struct sockaddr_in)); }
Socket::~Socket(void) { if (this->_fd) close(this->_fd); }
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

bool	Socket::init(int port) throw(std::exception) {
	// Create socket
	if ((this->_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		throw (Socket::FailToCreateException());
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
		throw (Socket::FailToBindException());
	}
	if (listen(this->_fd, 10) == -1) {
		throw (Socket::FailToListenException());
	}
	return (true);
}

// Exception
const char	*Socket::FailToCreateException::what(void) const throw() { return ("Socket: Fail to create"); }
const char	*Socket::FailToBindException::what(void) const throw() { return ("Socket: Fail to bind"); }
const char	*Socket::FailToListenException::what(void) const throw() { return ("Socket: Fail to listen"); }
const char	*Socket::FailToAcceptException::what(void) const throw() { return ("Socket: Fail to accept"); }
const char	*Socket::FailToControlException::what(void) const throw() { return ("Socket: Fail to control"); }