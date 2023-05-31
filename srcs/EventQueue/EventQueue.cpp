#include "../../incs/EventQueue/EventQueue.hpp"
#include <new>
#include <stddef.h>
#include <unistd.h>
#include <iostream>
#include <errno.h>
#include "../../incs/log/logger.hpp"
#include <fcntl.h>
// Constructor & Destructor
EventQueue *EventQueue::_instance = NULL;

EventQueue::EventQueue() {
	if ((this->_fd = kqueue()) == -1) {
		throw (FailToCreateException());
	}
}

EventQueue::~EventQueue() {
	if (this->_fd) close(this->_fd);
	delete _instance;
	_instance = NULL;
}


// Util
int		EventQueue::pullEvents(void) {
	Logger::getInstance().info("Enter Pull Event");
	int	ret = kevent(this->_fd, NULL, 0, this->_ev_list, MAX_EVENTS, NULL);
	
	Logger::getInstance().info("Pull Event");
	if (ret == -1) {
		throw (FailToGetEventException());
	}
	return (ret);
}
int		EventQueue::getEvQueFd(void) const { return (this->_fd); }
int		EventQueue::getEventFd(int idx) const { return (this->_ev_list[idx].ident); }
Event	*EventQueue::getEventData(int idx) const { 
	return static_cast<Event*>(this->_ev_list[idx].udata);
}

bool	EventQueue::pushEvent(Event* event) {
	Logger::getInstance().info("Add Event");
	try	{
		event->onboardQueue();
	} catch (const std::exception &e) {
		Logger::getInstance().error(e.what());
		return (false);
	} 
	return (true);
}

bool	EventQueue::popEvent(Event* event) {
	Logger::getInstance().info("delete Event");
	try	{
		event->offboardQueue();
	} catch (const std::exception &e) {
		Logger::getInstance().error(e.what());
		return (false);
	} 
	return (true);
	return (true);
}
struct kevent	*EventQueue::getEventList(void) { return (this->_ev_list); }
struct kevent 	*EventQueue::getEventSet(void) { return (this->_ev_set); }
struct kevent* EventQueue::getEvSetElementPtr(EvSetIndex index) { 
	return (&this->_ev_set[index]);
}
// Exception
const char	*EventQueue::FailToCreateException::what(void) const throw() { return ("EventQueue: Fail to create"); }
const char	*EventQueue::FailToControlException::what(void) const throw() { return ("EventQueue: Fail to control"); }
const char	*EventQueue::FailToGetEventException::what(void) const throw() { return ("EventQueue: Fail to get event"); }
const char	*EventQueue::TimeoutException::what(void) const throw() { return ("EventQueue: Timeout"); }