#include "../../incs/event/Event.hpp"
#include <new>

Event::Event(int connectionFd, EventHandler* EventHandler)  : _connection(new FdClass(connectionFd)),
 _eventHandler(EventHandler){}

Event::~Event(void) {}
const int &Event::getConnectionFd(void) const { return (this->_connection->getFd()); }
// To do: check Event Handler 랑 EventFactory구현 물 채워야 되는지
EventHandler::~EventHandler() {
}

EventFactory::EventFactory() {}	
EventFactory::~EventFactory() {}

//Exception
const char	*Event::FailToOnboardException::what(void) const throw() { return ("evnet: Fail to onboard"); }
const char	*Event::FailToOffboardException::what(void) const throw() { return ("evnet: Fail to offboard"); }