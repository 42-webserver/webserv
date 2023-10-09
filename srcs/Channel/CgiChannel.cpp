#include "../../incs/Channel/CgiChannel.hpp"

CgiChannel::CgiChannel(void) {
	for (int i = 0; i < Count; i++)
		this->_channel[i] = ft::shared_ptr<Channel>();
}

CgiChannel::~CgiChannel(void) {}

void CgiChannel::_destroy(e_pipe type) {
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
			Logger::getInstance().error("Fail to create pipe");
			throw (FailToCreateException());
		}
		this->_channel[i] = ft::shared_ptr<Channel>(new ByteStream(pipeFd[Read]));
		this->_channel[i]->setNonBlocking();
		this->_channel[i + 1] = ft::shared_ptr<Channel>(new ByteStream(pipeFd[Write]));
		this->_channel[i + 1]->setNonBlocking();
	}
}
void CgiChannel::dupCgiFd(void){
	if (dup2(this->_channel[CGI_READ]->getFd(), STDIN_FILENO) < 0) {
		Logger::getInstance().error("Fail to dup");
		throw (FailToDupException());
	}
	if (dup2(this->_channel[CGI_WRITE]->getFd(), STDOUT_FILENO) < 0) {
		Logger::getInstance().error("Fail to dup");
		throw (FailToDupException());
	}
}
void CgiChannel::clearCgiFd(void){
	this->_destroy(CGI_READ);
	this->_destroy(CGI_WRITE);
}

const char	*CgiChannel::FailToCreateException::what(void) const throw() { return ("CgiChannel: Fail to create"); }
const char	*CgiChannel::FailToDupException::what(void) const throw() { return ("CgiChannel: Fail to dup"); }