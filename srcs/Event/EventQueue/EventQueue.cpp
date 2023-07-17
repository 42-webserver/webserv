#include "../../../incs/Event/EventQueue/EventQueue.hpp"
#include <new>
#include <stddef.h>
#include <unistd.h>
#include <iostream>
#include <errno.h>
#include "../../../incs/Log/Logger.hpp"
#include <fcntl.h>



EventQueue	*EventQueue::_instance = NULL;

EventQueue::EventQueue(void) {
	std::cerr << "EventQueue::EventQueue" << std::endl;
	if ((this->_fd = kqueue()) == -1) {
		throw (FailToCreateException());
	}
}

EventQueue::~EventQueue(void) {
	// if (this->_fd) {
	// 	close(this->_fd);
	// }

	// delete EventQueue::_instance;
	// EventQueue::_instance = NULL;
}

EventQueue		&EventQueue::getInstance(void) {
	if (EventQueue::_instance == NULL) {
		EventQueue::_instance = new EventQueue();
	}
	return (*EventQueue::_instance);
}

int				EventQueue::getEventFd(int idx) const { return (this->_ev_list[idx].ident); }
int				EventQueue::getEventQueueFd(void) const { return (this->_fd); }
Event			*EventQueue::getEventData(int idx) const {  return static_cast<Event *>(this->_ev_list[idx].udata); }
struct kevent	*EventQueue::getEventList(void) { return (this->_ev_list); }
struct kevent 	*EventQueue::getEventSet(void) { return (&this->_ev_set); }
struct kevent	*EventQueue::getEventSetElementPtr() {  
	return (&this->_ev_set); }

int		EventQueue::pullEvents(void) {
	int	ret;

	Logger::getInstance().info("Enter Pull Event");
	ret = kevent(this->_fd, NULL, 0, this->_ev_list, MAX_EVENTS, NULL);	
	Logger::getInstance().info("Pull Event");
	if (ret == -1) {
		throw (FailToGetEventException());
	}
	return (ret);
}
bool	EventQueue::pushEvent(Event *event) {
	Logger::getInstance().info("Add Event");
	try	{
		event->onboardQueue();
	} catch (const std::exception &e) {
		Logger::getInstance().error(e.what());
		return (false);
	} 
	return (true);
}
bool	EventQueue::popEvent(Event *event) {
	Logger::getInstance().info("delete Event");
	try	{
		event->offboardQueue();
	} catch (const std::exception &e) {
		Logger::getInstance().error(e.what());
		return (false);
	} 
	return (true);
}

// Exception
const char	*EventQueue::FailToCreateException::what(void) const throw() { return ("EventQueue: Fail to create"); }
const char	*EventQueue::FailToControlException::what(void) const throw() { return ("EventQueue: Fail to control"); }
const char	*EventQueue::FailToGetEventException::what(void) const throw() { return ("EventQueue: Fail to get event"); }
const char	*EventQueue::TimeoutException::what(void) const throw() { return ("EventQueue: Timeout"); }