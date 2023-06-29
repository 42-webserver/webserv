#include "../../incs/Socket/Socket.hpp"
#include <new>
#include <sys/_endian.h>
#include <memory.h>
#include <sstream>

Socket::Socket(void): _fd(0), _enable(0) { memset(&this->_addr, 0, sizeof(struct sockaddr_in)); }
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
bool	Socket::build(const int port, const std::string &ip) throw(std::exception) {
	if ((this->_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		throw (FailToCreateException());
	}

	this->_enable = 1;
	setsockopt(this->_fd, SOL_SOCKET, SO_REUSEADDR, &this->_enable, sizeof(this->_enable));
	memset(&this->_addr, 0, sizeof(this->_addr));
	this->_addr.sin_family = AF_INET;
	this->_addr.sin_port = htons(port);
	this->_addr.sin_addr.s_addr = htonl(Socket::stringToNetworkByteOrder(ip));

	return (true);
}
bool	Socket::run(void) throw(std::exception) {
	if (bind(this->_fd, (struct sockaddr *)&this->_addr, sizeof(this->_addr)) == -1) {
		throw (FailToBindException());
	}
	if (listen(this->_fd, MAX_SIZE) == -1) {
		throw (FailToListenException());
	}
	return (true);
}
int		Socket::getFd(void) const { return (this->_fd); }

uint32_t	Socket::stringToNetworkByteOrder(const std::string &ip) {
    std::istringstream	iss(ip);
    std::string 		segment;
    uint32_t 			result = 0;
    uint32_t 			shift = 24;
	uint32_t			value;

    while (std::getline(iss, segment, '.')) {
        value = atoi(segment.c_str());
        result |= (value << shift);
        shift -= 8;
    }

    return (result);
}

// Exception
const char	*Socket::FailToCreateException::what(void) const throw() { return ("Socket: Fail to create"); }
const char	*Socket::FailToBindException::what(void) const throw() { return ("Socket: Fail to bind"); }
const char	*Socket::FailToListenException::what(void) const throw() { return ("Socket: Fail to listen"); }

std::ostream	&operator<<(std::ostream &os, const Socket &socket) {
	os << "\t\t\t\tSocket:" << '\n';
	os << "\t\t\t\t\t" << "fd: " << socket._fd;
	return (os);
}