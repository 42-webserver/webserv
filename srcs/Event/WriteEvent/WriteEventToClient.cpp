#include "../../../incs/Event/WriteEvent/WriteEventToClient.hpp"


WriteEventToClient::WriteEventToClient(ft::shared_ptr<Channel> channel,
ft::shared_ptr<VirtualServerManager> virtualServerManager
,ft::shared_ptr<HttpRequest> httpRequest):
WriteEvent(new WriteEventToClientHandler()), SingleStreamable(channel), DualStreamable(),
_virtualServerManager(virtualServerManager), _httpRequest(httpRequest) {}


WriteEventToClient::~WriteEventToClient(void) {}
void	WriteEventToClient::callEventHandler(void) {
	this->_event_handler->handleEvent(*this);
}
void	WriteEventToClient::onboardQueue(void){
	Event *event = this;

	try {
		this->getChannel().get()->setNonBlocking();
		this->_onboardWrite(event, this->getFd());
	}
	catch (...) {
		Logger::getInstance().error("Fail to onboard Write Event");
		throw ;
	}
}

void	WriteEventToClient::offboardQueue(void){
	try {
		this->_offboardWrite(this, this->getFd());
	}
	catch (...) {
		Logger::getInstance().error("Fail to offboard Write Event");
		throw ;
	}	
}

const ft::shared_ptr<VirtualServerManager>	
&WriteEventToClient::getVirtualServerManger(void) const { return (this->_virtualServerManager); }