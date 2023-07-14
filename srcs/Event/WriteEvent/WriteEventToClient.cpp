#include "../../../incs/Event/WriteEvent/WriteEventToClient.hpp"
#include "../../../incs/Event/WriteEvent/WriteEventToClientHandler.hpp"
#include "../../../incs/Event/EventQueue/EventQueue.hpp"

WriteEventToClient::WriteEventToClient(ft::shared_ptr<Channel> channel,
 ft::shared_ptr<VirtualServerManager> virtualServerManager
 , ft::shared_ptr<HttpRequest> httpRequest):
WriteEvent(channel, new WriteEventToClientHandler()),
 _virtualServerManager(virtualServerManager),
 _httpRequest(httpRequest) {}
WriteEventToClient::~WriteEventToClient(void) {}
void	WriteEventToClient::callEventHandler(void) {
	this->_event_handler->handleEvent(*this);
}
void	WriteEventToClient::onboardQueue(void) throw (std::exception) {
	EventQueue &event_queue = EventQueue::getInstance();
	int fd = this->getFd();
	Event *event = this;

	Logger::getInstance().info("onboard Write Event");
	EV_SET(
		event_queue.getEventSetElementPtr(EventQueue::WRITE_SET),
		fd,
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
		throw (FailToOnboardException());
	}}

void	WriteEventToClient::offboardQueue(void) throw (std::exception) {
	EventQueue &event_queue = EventQueue::getInstance();

	Logger::getInstance().info("Remove Write Event");
	EV_SET(
		event_queue.getEventSetElementPtr(EventQueue::WRITE_SET),
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