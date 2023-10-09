#include "../../../incs/Event/ReadEvent/ReadEventFromClient.hpp"
#include "../../../incs/Event/ReadEvent/ReadEventFromClientHandler.hpp"
#include "../../../incs/Event/EventQueue/EventQueue.hpp"

ReadEventFromClient::ReadEventFromClient(ft::shared_ptr<Channel> channel, 
	ft::shared_ptr<VirtualServerManager> virtualServerManager):
	ReadEvent(channel, new ReadEventFromClientHandler()),
	_virtualServerManager(virtualServerManager)
	{}
ReadEventFromClient::~ReadEventFromClient(void) {}
void	ReadEventFromClient::callEventHandler(void) { this->_event_handler->handleEvent(*this); }
void	ReadEventFromClient::onboardQueue() {
	EventQueue	&event_queue = EventQueue::getInstance();
	int			fd = this->getFd();
	Event		*event = this;

	try {
		event->getChannel()->setNonBlocking();
	} catch (const std::exception &e) {
		Logger::getInstance().error("{} {}", 2, "ReadEventClient", e.what());
		throw (FailToOnboardException());
		return ;
	}

	EV_SET(
		event_queue.getEventSetElementPtr(),
		fd,
		EVFILT_READ,
		EV_ADD | EV_ENABLE,
		0,
		0,
		event
	);

	if (kevent(event_queue.getEventQueueFd(), event_queue.getEventSet(), 1, NULL, 0, NULL) == -1) {
		Logger::getInstance().error("{} {}", 
		2, 
		"ReadEventClient : kevents", strerror(errno));
		throw (FailToOnboardException());
	}}
void	ReadEventFromClient::offboardQueue() {
	EventQueue &event_queue = EventQueue::getInstance();

	Logger::getInstance().info("Remove Read Event");
	EV_SET(
		event_queue.getEventSetElementPtr(),
		this->getFd(),
		EVFILT_READ,
		EV_DELETE,
		0,
		0,
		this
	);


	if (kevent(event_queue.getEventQueueFd(), event_queue.getEventSet(), 1, NULL, 0, NULL) == -1) {
		throw (Event::FailToOffboardException());
	}
		delete this;

}

const ft::shared_ptr<VirtualServerManager>	
&ReadEventFromClient::getVirtualServerManger(void) const { return (this->_virtualServerManager); }