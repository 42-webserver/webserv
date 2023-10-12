#include "../../../incs/Event/ReadEvent/ReadEventFromClient.hpp"

ReadEventFromClient::ReadEventFromClient(ft::shared_ptr<Channel> channel, 
	ft::shared_ptr<VirtualServerManager> virtualServerManager):
	ReadEvent(new ReadEventFromClientHandler()),
	SingleStreamable(channel),
	_virtualServerManager(virtualServerManager),
	_client()
	{}
ReadEventFromClient::~ReadEventFromClient(void) {}
void	ReadEventFromClient::callEventHandler(void) { this->_event_handler->handleEvent(*this); }
void	ReadEventFromClient::onboardQueue() {
	int			fd = this->getFd();
	Event		*event = this;

	try {
		this->getChannel()->setNonBlocking();
		this->_onboardRead(event, fd);
	} catch (KqueueError &e) {
		Logger::getInstance().error("{} {}", 2, "ReadEventClient", e.what());
		throw (KqueueError());
	}
	catch (...) {
		Logger::getInstance().error("{} {}", 2, "ReadEventClient", "Fail to onboard Read Event");
		throw ;
	}
}
void	ReadEventFromClient::offboardQueue() {

	try {
		this->_offboardRead(this, this->getFd());
	} catch (const std::exception &e) {
		Logger::getInstance().error("{} {}", 2, "ReadEventClient", e.what());
		throw (KqueueError());
	}

}
bool ReadEventFromClient::canRead(void) const {
	if (_client->isQueueMax())
		return (false);
	return (true);
}
const ft::shared_ptr<VirtualServerManager>	&ReadEventFromClient::getVirtualServerManger(void) const { return (this->_virtualServerManager); }

void	ReadEventFromClient::addRequest(ft::shared_ptr<HttpRequest> request) {
	this->_client->addRequest(request);
}

bool	ReadEventFromClient::queryInEventQueue(e_client_queue_state state) {
	return (this->_client->isInEventQueue(state));
}