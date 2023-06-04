#include "../../incs/Event/ListenEvent.hpp"
#include <new>
#include <unistd.h>
#include <fcntl.h>
#include "../../incs/Event/ReadEvent.hpp"
ListenEvent::ListenEvent(int fd, EventHandler *listen_event_handler, 
const VirtualServerMap::TargetMap *TargetMap): Event(fd, listen_event_handler), 
_TargetMap(TargetMap) {}

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

VirtualServerMap::TargetMap	*ListenEvent::getTargetMap(void) const { return (const_cast<VirtualServerMap::TargetMap *>(this->_TargetMap)); }
// To do: 
// Listen Event Handler 
ListenEventHandler::ListenEventHandler(void) {};
ListenEventHandler::~ListenEventHandler(void) {};

int		ListenEventHandler::connectClient(int server_fd) const throw(std::exception) {
	int	client_fd;
	
	if ((client_fd = ::accept(server_fd, (struct sockaddr *) NULL, NULL)) == -1) {
		Logger::getInstance().error("Fail to accept client");
		throw (FailToAcceptException());
		return (-1);
	}

	return (client_fd);	
}
void	ListenEventHandler::handleEvent(Event &event) throw (std::exception) {
	Logger		&log = Logger::getInstance();
	int 		client_fd;

	try {
		client_fd = connectClient(event.getFd());
		if (client_fd != -1) {
			log.info("Client connected");
		}
	} catch (const std::exception &e) {
		log.error(e.what());
		return ;
	}

	EventFactory &factory = ReadEventClientFactory::getInstance();
	try {
		EventQueue &event_queue = EventQueue::getInstance();
		ListenEvent *listenEvent = dynamic_cast<ListenEvent *>(&event);

		EventDto event_dto(client_fd, listenEvent->getTargetMap());
		event_queue.pushEvent(factory.createEvent(event_dto));
	} catch (const std::exception &e) {
		log.error(e.what());
		//check: 이렇게 하는게 맞을지 생각
	}
}

ListenEventFactory::ListenEventFactory(void) {}
ListenEventFactory::~ListenEventFactory(void) {}
ListenEventFactory	&ListenEventFactory::getInstance(void) {
	static ListenEventFactory	instance;

	return (instance);
}

Event	*ListenEventFactory::createEvent(const EventDto &EventDto) const {
	EventHandler	*event_handler = new ListenEventHandler();
	ListenEvent		*event = new ListenEvent(EventDto.getFd(), 
	event_handler, EventDto.getTargetMap());

	return (event);
}


// Method 구현: 꼭 vtable신경
// Exception
const char	*ListenEvent::FailToAcceptException::what(void) const throw() { return ("ListenEvent: Fail to accept"); }
const char	*ListenEvent::FailToControlException::what(void) const throw() { return ("ListenEvent: Fail to control"); }
const char	*ListenEventHandler::FailToAcceptException::what(void) const throw() { return ("ListenEventHandler: Fail to accept"); }
const char	*ListenEventHandler::FailToControlException::what(void) const throw() { return ("ListenEventHandler: Fail to control"); }