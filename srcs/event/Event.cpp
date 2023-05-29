#include "../../incs/event/Event.hpp"
#include <new>

Event::Event(int connectionFd, EventHandler* EventHandler, ft::shared_ptr<Kqueue> kqueue)  : _connection(new FdClass(connectionFd)),
 _eventHandler(EventHandler), _kqueue(kqueue){}

Event::~Event(void) {}

// To do: check Event Handler 랑 EventFactory구현 물 채워야 되는지
EventHandler::~EventHandler() {
}

EventFactory::EventFactory() {}	
EventFactory::~EventFactory() {}