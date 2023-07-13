#include "../../../incs/Event/EventBase/EventFactory.hpp"
#include "../../../incs/Event/ReadEvent/ReadEventFromClient.hpp"
#include "../../../incs/Event/WriteEvent/WriteEventToClient.hpp"
#include "../../../incs/Event/ReadEvent/ReadEventFromCgi.hpp"
#include "../../../incs/Event/ListenEvent/ListenEvent.hpp"
// #include "../../../incs/Event/WriteEvent/WriteEventToCgi.hpp"

EventFactory *EventFactory::_instance = NULL;
EventFactory::EventFactory(void) {}	
EventFactory::~EventFactory(void) {}
EventFactory &EventFactory::getInstance(void) {
	if (_instance == 0)
		_instance = new EventFactory();
	return (*_instance);
}
void EventFactory::DeleteInstance(void) {
	if (_instance != 0)
		delete _instance;
	_instance = NULL;
}
Event *EventFactory::createEvent(ft::EventType eventType, EventDto &eventDto) throw (FailToEventCreateException) {
	switch (eventType) {
		case ft::READ_EVENT_FROM_CLIENT:
			return (new ReadEventFromClient(eventDto.getFd(),
			 eventDto.getVirtualServerManager())
			);
		case ft::WRITE_EVENT_TO_CLIENT:
			return (new WriteEventToClient(eventDto.getFd(),
			 eventDto.getVirtualServerManager(),
			 eventDto.getHttpRequest())
			);
		// case ft::READ_EVENT_FROM_CGI:
		// 	return (new ReadEventFromCgi(eventDto));
		// case ft::WRITE_EVENT_TO_CGI:
		// 	return (new WriteEventToCgi(eventDto));
		case ft::LISTEN_EVENT:
			return (new ListenEvent(eventDto.getFd(),
			 eventDto.getVirtualServerManager())
			);
		default:
			throw (FailToEventCreateException());
	}
}

// exception
const char *EventFactory::FailToEventCreateException::what() const throw() {
	return ("Fail to create Event");
}