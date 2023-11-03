#include "../../../incs/Event/ReadEvent/ReadEventFromClient.hpp"

ReadEventFromClient::ReadEventFromClient(ft::shared_ptr<Channel> channel, 
	ft::shared_ptr<VirtualServerManager> virtualServerManager):
	ReadEvent(new ReadEventFromClientHandler()),
	SingleStreamable(channel),
	_virtualServerManager(virtualServerManager),
	_client(new Client(Read))
	{}
ReadEventFromClient::~ReadEventFromClient(void) {}
void	ReadEventFromClient::callEventHandler(void) { this->_event_handler->handleEvent(*this); }
void	ReadEventFromClient::onboardQueue() {
	int			fd = this->getFd();
	Event		*event = this;
	
		std::cerr << "ReadEventFromClient::onboardQueue()" << std::endl;

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

	std::cerr << "ReadEventFromClient::offboardQueue()" << std::endl;
	try {
		this->_client->removeClientEventQueueState(Read);
		this->_offboardRead(this, this->getFd());
		//To do: check this
		std::cerr << "ReadEventFromClient::offboardQueue() end" << std::endl;
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

e_client_event_queue_state ReadEventFromClient::queryClientEventQueueState(void) {
	return (this->_client->queryClientEventQueueState());
}

bool ReadEventFromClient::isRequestEmpty(void) {
	return (this->_client->isRequestEmpty());
}

bool ReadEventFromClient::isEventQueueTurnOn(e_client_event_queue_state state) {
	return (this->_client->isEventQueueStateTurnOn(state));
}

ft::shared_ptr<Client> ReadEventFromClient::getClient(void) {
	return (this->_client);
}

bool ReadEventFromClient::isClientDie(void) {
	return (this->_client->isClientDie());
}