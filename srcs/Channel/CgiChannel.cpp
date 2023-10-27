#include "../../incs/Channel/CgiChannel.hpp"
#include <Http/Exception/InternalServerErrorException.hpp>

CgiChannel::CgiChannel(void) {
	for (int i = 0; i < Count; i++)
		this->_channel[i] = ft::shared_ptr<Channel>();
}

CgiChannel::~CgiChannel(void) {}

void CgiChannel::destroy(e_pipe type) {
	this->_channel[type] = ft::shared_ptr<Channel>();
}

int CgiChannel::getFd(e_pipe pipe) const {
	if (this->_channel[pipe].get() == NULL)
		return -1;
	return this->_channel[pipe]->getFd();
}

void CgiChannel::build(void) {
	int pipeFd[2];

	for (int i = 0; i < Count; i += 2) {
		if (pipe(pipeFd) == -1) {
			// Logger::getInstance().error("Fail to create pipe");
			throw std::runtime_error("Fail to create pipe");
		}
		try {
			this->_channel[i] = ft::shared_ptr<Channel>(new ByteStream(pipeFd[Read]));
			this->_channel[i]->setNonBlocking();
			this->_channel[i + 1] = ft::shared_ptr<Channel>(new ByteStream(pipeFd[Write]));
			this->_channel[i + 1]->setNonBlocking();
		}
		catch (const std::exception &e) {
			// Logger::getInstance().error(e.what());
			throw (InternalServerErrorException());
		}
	}
}
void CgiChannel::_dupFdInCgiProcess(void){
	if (dup2(this->_channel[e_cgi_read]->getFd(), STDIN_FILENO) < 0) {
		Logger::getInstance().error("Fail to dup");
		throw (FailToDupException());
	}
	if (dup2(this->_channel[e_cgi_write]->getFd(), STDOUT_FILENO) < 0) {
		Logger::getInstance().error("Fail to dup");
		throw (FailToDupException());
	}
}

void CgiChannel::_closeServerSideFd(void) {
	this->_channel[e_server_read] = ft::shared_ptr<Channel>();
	this->_channel[e_server_write] = ft::shared_ptr<Channel>();
}

void CgiChannel::_closeCgiSideFd(void) {
	this->_channel[e_cgi_read] = ft::shared_ptr<Channel>();
	this->_channel[e_cgi_write] = ft::shared_ptr<Channel>();
}

ft::shared_ptr<Channel> &CgiChannel::getChannel(e_pipe type) {
	return (this->_channel[type]);
}
// void CgiChannel::_setNonBlockServerSideFd(void) {
// 	this->_channel[SERVER_READ]->setNonBlocking();
// 	this->_channel[SERVER_WRITE]->setNonBlocking();
// }
const char	*CgiChannel::FailToCreateException::what(void) const throw() { return ("CgiChannel: Fail to create"); }
const char	*CgiChannel::FailToDupException::what(void) const throw() { return ("CgiChannel: Fail to dup"); }