#include "../../incs/Channel/Socket.hpp"
#include <new>
#include <sys/_endian.h>
#include <memory.h>
#include <sstream>
#include "../../incs/Log/Logger.hpp"
Socket::Socket(void): Channel(), _enable(0), _addr(sockaddr_in()) { memset(&this->_addr, 0, sizeof(struct sockaddr_in)); }
Socket::Socket(int fd): Channel(fd), _enable(1), _addr(sockaddr_in()) { }
Socket::Socket(int fd, std::string clientIp): Channel(fd), _enable(1), _addr(sockaddr_in()), _clientIp(clientIp) { }
Socket::Socket(const Socket &ref): Channel(),_enable(ref._enable), _addr(ref._addr) {}
Socket::~Socket(void) {
	// Logger::getInstance().info("Socket is destroyed");
	this->destroyChannelFd();
}
Socket	&Socket::operator=(const Socket &rhs) {
	if (this != &rhs) {
		this->~Socket();
		new (this) Socket(rhs);
	}
	return (*this);
}

void Socket::inJectChannelFd(int fd) { 
	setChannelFd(fd);
}
// Utils
bool	Socket::build(const int port, const std::string &ip){
	int socketFd;
	if ((socketFd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		throw (FailToCreateException());
	}

	this->_enable = 1;
	setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, &this->_enable, sizeof(this->_enable));
	memset(&this->_addr, 0, sizeof(this->_addr));
	this->_addr.sin_family = AF_INET;
	this->_addr.sin_port = htons(port);
	this->_addr.sin_addr.s_addr = htonl(Socket::stringToNetworkByteOrder(ip));

	this->inJectChannelFd(socketFd);
	return (true);
}
bool	Socket::run(void)  {
	if (bind(this->getFd(), (struct sockaddr *)&this->_addr, sizeof(this->_addr)) == -1) {
		Logger::getInstance().error(strerror(errno));
		std::cerr << "Socket::run() catch" << strerror(errno) << std::endl;
		throw (FailToBindException());
	}
	if (listen(this->getFd(), MAX_SIZE) == -1) {
		throw (FailToListenException());
	}
	return (true);
}

const std::string& Socket::getClientIp() const {
	return _clientIp;
}
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
	os << "\t\t\t\t\t" << "fd: " << socket.getFd();
	return (os);
}

std::string Socket::custom_inet_ntoa(struct in_addr in){
    unsigned char* bytes = (unsigned char*)&in;
    std::ostringstream oss;
    oss << (int)bytes[0] << "." << (int)bytes[1] << "." << (int)bytes[2] << "." << (int)bytes[3];
    return oss.str();
}
