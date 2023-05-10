#include "../../incs/socket/PassiveSocket.hpp"
#include <new>
#include <sys/_endian.h>
#include <unistd.h>
#include <fcntl.h>
#include <memory.h>

// Constructor & Destructor
PassiveSocket::PassiveSocket(void): Socket(), _enable(0) { memset(&this->_addr, 0, sizeof(struct sockaddr_in)); }
PassiveSocket::PassiveSocket(int port): Socket(socket(AF_INET, SOCK_STREAM, 0)), _enable(0) {
	// Create socket
	if (this->_fd == -1) {
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
PassiveSocket::PassiveSocket(const PassiveSocket &ref): Socket(ref), _enable(ref._enable), _addr(ref._addr) {}
PassiveSocket::~PassiveSocket(void) {}
PassiveSocket	&PassiveSocket::operator=(const PassiveSocket &rhs) {
	if (this != &rhs) {
		this->~PassiveSocket();
		new (this) PassiveSocket(rhs);
	}
	return (*this);
}

// Utils
int	PassiveSocket::accept(void) throw(std::exception) {
	int					client_fd;
	struct sockaddr_in	client_addr;
	socklen_t			client_addr_len;
	int					flags;

	client_addr_len = sizeof(client_addr);
	if ((client_fd = ::accept(this->_fd, (struct sockaddr *)&client_addr, &client_addr_len)) == -1) {
		throw (FailToAcceptException());
	}
	if ((flags = fcntl(client_fd, F_GETFL, 0)) == -1) {
		close(client_fd);
		throw (FailToControlException());
	}
	if (fcntl(client_fd, F_SETFL, flags | O_NONBLOCK) == -1) {
		close(client_fd);
		throw (FailToControlException());
	}
	return (client_fd);
}

// Exception
const char	*PassiveSocket::FailToCreateException::what(void) const throw() { return ("PassiveSocket: Fail to create"); }
const char	*PassiveSocket::FailToBindException::what(void) const throw() { return ("PassiveSocket: Fail to bind"); }
const char	*PassiveSocket::FailToListenException::what(void) const throw() { return ("PassiveSocket: Fail to listen"); }
const char	*PassiveSocket::FailToAcceptException::what(void) const throw() { return ("PassiveSocket: Fail to accept"); }
const char	*PassiveSocket::FailToControlException::what(void) const throw() { return ("PassiveSocket: Fail to control"); }