#include "../../incs/Channel/Channel.hpp"

Channel::Channel(void) : _ChannelFd(-1) {}
Channel::Channel(int fd) : _ChannelFd(fd) {}
Channel::~Channel(void) {
	close(this->_ChannelFd);
}
Channel::Channel(const Channel &ref) { *this = ref; }
Channel	&Channel::operator=(const Channel &rhs) {
	if (this != &rhs) {
		this->_ChannelFd = rhs._ChannelFd;
	}
	return (*this);
}
void	Channel::destroyChannelFd(void) {
	if (this->_ChannelFd != -1) {
		close(this->_ChannelFd);
		this->_ChannelFd = -1;
	}
}
void	Channel::setNonBlocking(void){
	int flags;

	int fd = this->getFd();
	if ((flags = fcntl(fd, F_GETFL, 0)) == -1) {
		throw (FailToNonBlockException());
	}

	if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
		throw (FailToNonBlockException());
	}
}
void Channel::setChannelFd(int fd) { this->_ChannelFd = fd; }
int	Channel::getFd() const { return this->_ChannelFd; }
const char	*Channel::FailToNonBlockException::what(void) const throw() { return ("Channel: Fail to setNonBlocking"); }