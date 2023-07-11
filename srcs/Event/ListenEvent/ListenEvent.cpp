#include "../../../incs/Event/ListenEvent/ListenEvent.hpp"
#include <new>
#include <unistd.h>
#include <fcntl.h>
#include "../../../incs/Event/ReadEvent/ReadEvent.hpp"

ListenEvent::ListenEvent(int fd, EventHandler *listen_event_handler, ft::shared_ptr<VirtualServerManager> physical_server):
	Event(fd, listen_event_handler),
	_physical_server(physical_server)
	{}

ListenEvent::~ListenEvent(void) {}

void	ListenEvent::callEventHandler(void) { this->_event_handler->handleEvent(*this); }
void	ListenEvent::onboardQueue(void) throw (std::exception) {
	EventQueue	&event_queue = EventQueue::getInstance();
	int			fd = this->getFd();
	Event		*event = this;

	Logger::getInstance().info("onboard Listen Event");
	
	try {
		event->getFileDescriptor()->setNonBlocking();
	} catch (const std::exception &e) {
		Logger::getInstance().error(e.what());
		Logger::getInstance().error("Fail to accept client");
		throw (Event::FailToOnboardException());
		return ;
	}

	EV_SET(
		event_queue.getEventSetElementPtr(READ_SET),
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
		event_queue.getEventSetElementPtr(READ_SET),
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

ft::shared_ptr<VirtualServerManager>	ListenEvent::getPhysicalServer(void) const { return (this->_physical_server); }
// Exception
const char	*ListenEvent::FailToAcceptException::what(void) const throw() { return ("ListenEvent: Fail to accept"); }
const char	*ListenEvent::FailToControlException::what(void) const throw() { return ("ListenEvent: Fail to control"); }
