#include "../../incs/event/Event.hpp"
#include <new>

Event::Event(int connectionFd, EventHandler* EventHandler)  : _connection(new FdClass(connectionFd)),
 _eventHandler(EventHandler) {}

Event::~Event(void) {}

EventHandler::~EventHandler() {
}
