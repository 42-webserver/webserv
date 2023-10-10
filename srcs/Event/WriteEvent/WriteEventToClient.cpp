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
	EventQueue &event_queue = EventQueue::getInstance();
	Event *event = this;

	Logger::getInstance().info("onboard Write Event");
	EV_SET(
		event_queue.getEventSetElementPtr(),
		this->getFd(),
		EVFILT_WRITE,
		EV_ADD | EV_ENABLE,
		0,
		0,
		event
	);

	if (kevent(event_queue.getEventQueueFd(), event_queue.getEventSet(), 1, NULL, 0, NULL) == -1) {
		Logger::getInstance().error("{} {}", 
		2, 
		"WriteEventClient : kevents", strerror(errno));
		std::cerr << "WriteEventClient : kevents" << strerror(errno) << std::endl;
		std::cerr << event_queue.getEventQueueFd() << std::endl;
		std::cerr << event_queue.getEventSet() << std::endl;
		throw (FailToOnboardException());
	}
}

void	WriteEventToClient::offboardQueue(void){
	EventQueue &event_queue = EventQueue::getInstance();

	Logger::getInstance().info("Remove Write Event");
	EV_SET(
		event_queue.getEventSetElementPtr(),
		this->getFd(),
		EVFILT_WRITE,
		EV_DELETE,
		0,
		0,
		this
	);

	delete this;

	if (kevent(event_queue.getEventQueueFd(), event_queue.getEventSet(), 1, NULL, 0, NULL) == -1) {
		throw (Event::FailToOffboardException());
	}	
}

const ft::shared_ptr<VirtualServerManager>	
&WriteEventToClient::getVirtualServerManger(void) const { return (this->_virtualServerManager); }