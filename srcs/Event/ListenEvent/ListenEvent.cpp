#include "../../../incs/Event/ListenEvent/ListenEvent.hpp"
#include <unistd.h>
#include <fcntl.h>
#include "../../../incs/Event/ReadEvent/ReadEvent.hpp"
#include "../../../incs/Event/ListenEvent/ListenEventHandler.hpp"
#include "../../../incs/Event/EventQueue/EventQueue.hpp"

ListenEvent::ListenEvent(ft::shared_ptr<Channel> channel, ft::shared_ptr<VirtualServerManager> virtual_server_manager):
Event(channel, new ListenEventHandler()), _virtualServerManager(virtual_server_manager)
	{}

ListenEvent::~ListenEvent(void) {}

void	ListenEvent::callEventHandler(void) { this->_event_handler->handleEvent(*this); }
void	ListenEvent::onboardQueue(void) throw (std::exception) {
	EventQueue	&event_queue = EventQueue::getInstance();
	int			fd = this->getFd();
	Event		*event = this;

	Logger::getInstance().info("onboard Listen Event");
	
	try {
		(event->getChannel()).get()->setNonBlocking();
	} catch (const std::exception &e) {
		Logger::getInstance().error(e.what());
		Logger::getInstance().error("Fail to accept client");
		throw (Event::FailToOnboardException());
		return ;
	}

	EV_SET(
		event_queue.getEventSetElementPtr(EventQueue::READ_SET),
		fd,
		EVFILT_READ,
		EV_ADD | EV_ENABLE,
		0,
		0,
		event
	);

	if (kevent(event_queue.getEventQueueFd(), event_queue.getEventSet(), 1, NULL, 0, NULL) == -1) {
		perror("kevent");
		throw (FailToOnboardException());
	}
}
void	ListenEvent::offboardQueue(void) throw (std::exception) {
	EventQueue &event_queue = EventQueue::getInstance();

	Logger::getInstance().info("Remove Listen Event");
	EV_SET(
		event_queue.getEventSetElementPtr(EventQueue::READ_SET),
		this->getFd(),
		EVFILT_READ,
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

ft::shared_ptr<VirtualServerManager>	ListenEvent::getVirtualServerManager(void) const { return (this->_virtualServerManager); }
// Exception
const char	*ListenEvent::FailToAcceptException::what(void) const throw() { return ("ListenEvent: Fail to accept"); }
const char	*ListenEvent::FailToControlException::what(void) const throw() { return ("ListenEvent: Fail to control"); }
