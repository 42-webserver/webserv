#include "../../incs/Event/ReadEvent.hpp"
#include <new>
#include <string>

// READ EVENT start!
ReadEvent::ReadEvent(int fd, EventHandler *read_event_handler): Event(fd, read_event_handler), _buffer(ReadEvent::BUF_SIZE) {}
ReadEvent::~ReadEvent(void) {}

// To do: check Read Buffer need 
// const std::vector<char>	&ReadEvent::getBuffer(void) const { return (this->_buffer); }

ReadEventHandler::~ReadEventHandler(void) {
	// Cleanup code here
}


// READ EVENT End!

// Read EvClient start!
//	To do : check ReadEventClient

ReadEventClient::ReadEventClient(int fd, EventHandler *read_event_client_handler,
const VirtualServerMap::TargetMap *TargetMap) : 
ReadEvent(fd, read_event_client_handler), _TargetMap(TargetMap) {}
ReadEventClient::~ReadEventClient(void) {}
const VirtualServerMap::TargetMap	*ReadEventClient::getTargetMap(void) const { return (this->_TargetMap); }
void	ReadEventClient::callEventHandler(void) { this->_event_handler->handleEvent(*this); }
void	ReadEventClient::onboardQueue() throw (std::exception) {
	EventQueue	&event_queue = EventQueue::getInstance();
	int			fd = this->getFd();
	Event		*event = this;

	Logger::getInstance().info("onboard Read Event");
	try {
		event->getFileDescriptor()->setNonBlocking();
	} catch (const std::exception &e) {
		Logger::getInstance().error("{} {}", 2, "ReadEventClient", e.what());
		throw (FailToOnboardException());
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
		Logger::getInstance().error("{} {}", 
		2, 
		"ReadEventClient : kevents", strerror(errno));
		throw (FailToOnboardException());
	}}
void	ReadEventClient::offboardQueue() throw (std::exception) {
	EventQueue &event_queue = EventQueue::getInstance();

	Logger::getInstance().info("Remove Read Event");
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

// Read EvClient end!

// Read EvClientHandler start!
// To do : implement ReadEventClientHandler
ReadEventClientHandler::ReadEventClientHandler(void) {}
void ReadEventClientHandler::handleEvent(Event &event) {
	//test용
	event.callEventHandler();
	std::cout << "hi?\n";
	//
}

ReadEventClientHandler::~ReadEventClientHandler(void) {}
// Read EvClientHandler

// Read EvClientFactory start!
ReadEventClientFactory::ReadEventClientFactory(void) {}
ReadEventClientFactory &ReadEventClientFactory::getInstance(void) {
	static ReadEventClientFactory instance;
	return (instance);
}

Event	*ReadEventClientFactory::createEvent(const EventDto &event_dto) {
	EventHandler	*event_handler = new ReadEventClientHandler();
	ReadEventClient	*event = new ReadEventClient(
	event_dto.getFd(), 
	event_handler, 
	event_dto.getTargetMap());

	return (event);
}