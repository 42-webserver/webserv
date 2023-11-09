#include "../../incs/Channel/CgiChannel.hpp"
#include <Http/Exception/InternalServerErrorException.hpp>

CgiChannel::CgiChannel(void){}

CgiChannel::~CgiChannel(void) {}

void CgiChannel::destroy(e_pipe type) {
	close(this->_pipeFd[type]);
}

int CgiChannel::getFd(e_pipe pipe) {
	return (this->_pipeFd[pipe]);
}

void CgiChannel::build(void) {
	int pipeFd[2];

	for (int i = 0; i < Count; i += 2) {
		try {
			if (pipe(pipeFd) == -1) {
				throw std::runtime_error("Fail to create pipe");
			}		
			this->_pipeFd[i] = pipeFd[Read];
			this->_pipeFd[i + 1] = pipeFd[Write];
		}
		catch (const std::exception &e) {
			Logger::getInstance().error(e.what());
			throw (InternalServerErrorException());
		}
	}
}
void CgiChannel::_dupFdInCgiProcess(void){
	if (dup2(_pipeFd[e_cgi_read], STDIN_FILENO) < 0) {
		Logger::getInstance().error("Fail to dup");
		throw (FailToDupException());
	}
	if (dup2(_pipeFd[e_cgi_write], STDOUT_FILENO) < 0) {
		Logger::getInstance().error("Fail to dup");
		throw (FailToDupException());
	}
	// if (dup2(getChannel(e_cgi_read)->getFd(), STDIN_FILENO) < 0) {
	// 	// Logger::getInstance().error("Fail to dup");
	// 	throw (FailToDupException());
	// }
	// if (dup2(getChannel(e_cgi_write)->getFd(), STDOUT_FILENO) < 0) {
	// 	// Logger::getInstance().error("Fail to dup");
	// 	throw (FailToDupException());
	// }
}

void CgiChannel::_closeServerSideFd(void) {
	// this->_channel[e_server_read] = ft::shared_ptr<Channel>();
	// this->_channel[e_server_write] = ft::shared_ptr<Channel>();
	destroy(e_server_read);
	destroy(e_server_write);
}

void CgiChannel::_closeCgiSideFd(void) {
	// this->_channel[e_cgi_read] = ft::shared_ptr<Channel>();
	// this->_channel[e_cgi_write] = ft::shared_ptr<Channel>();
	destroy(e_cgi_read);
	destroy(e_cgi_write);
}

int CgiChannel::getPipFd(e_pipe type) {
	return (this->_pipeFd[type]);
}
// void CgiChannel::_setNonBlockServerSideFd(void) {
// 	this->_channel[SERVER_READ]->setNonBlocking();
// 	this->_channel[SERVER_WRITE]->setNonBlocking();
// }
const char	*CgiChannel::FailToCreateException::what(void) const throw() { return ("CgiChannel: Fail to create"); }
const char	*CgiChannel::FailToDupException::what(void) const throw() { return ("CgiChannel: Fail to dup"); }