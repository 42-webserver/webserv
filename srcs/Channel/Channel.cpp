#include "../../incs/Channel/Channel.hpp"

Channel::Channel(void) : _ChannelFd(-1) {}
Channel::~Channel(void) {}
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
void	Channel::setNonBlocking(void) throw (std::exception) {
	int flags;

	if ((flags = fcntl(this->getFd(), F_GETFL, 0)) == -1) {
		Logger::getInstance().error("Fail to control client");
		close(this->getFd());
		throw (FailToNonBlockException());
		return ;
	}

	if (fcntl(this->getFd(), F_SETFL, flags | O_NONBLOCK) == -1) {
		Logger::getInstance().error("Fail to control client");
		close(this->getFd());
		throw (FailToNonBlockException());
	}
}
void Channel::setChannelFd(int fd) { this->_ChannelFd = fd; }
int	Channel::getFd() const { return this->_ChannelFd; }
const char	*Channel::FailToNonBlockException::what(void) const throw() { return ("Channel: Fail to setNonBlocking"); }