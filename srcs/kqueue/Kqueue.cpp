#include "../../incs/kqueue/Kqueue.hpp"
#include <new>
#include <stddef.h>
#include <unistd.h>
#include <iostream>
#include <errno.h>

// Constructor & Destructor
Kqueue::Kqueue(void) {
	if ((this->_fd = kqueue()) == -1) {
		throw (FailToCreateException());
	}
	memset(this->_ev_set, 0, 2 * sizeof(struct kevent));
	memset(this->_ev_list, 0, MAX_EVENTS * sizeof(struct kevent));
}
Kqueue::Kqueue(const Kqueue &ref): _fd(ref._fd) {
	for (size_t i=0; i<2; i++) {
		this->_ev_set[i] = ref._ev_set[i];
	}
	for (size_t i=0; i<MAX_EVENTS; i++) {
		this->_ev_list[i] = ref._ev_list[i];
	}
}
Kqueue::~Kqueue(void) { if (this->_fd) close(this->_fd); }
Kqueue	&Kqueue::operator=(const Kqueue &rhs) {
	if (this != &rhs) {
		this->~Kqueue();
		new (this) Kqueue(rhs);
	}
	return (*this);
}

// Util
int		Kqueue::pullEvents(void) {
	int	ret = kevent(this->_fd, NULL, 0, this->_ev_list, MAX_EVENTS, NULL);
	
	if (ret == -1) {
		throw (FailToGetEventException());
	}
	return (ret);
}
int		Kqueue::getEventFd(int idx) const { return (this->_ev_list[idx].ident); }
Event	*Kqueue::getEventData(int idx) const { 
	return static_cast<Event*>(this->_ev_list[idx].udata);
}
bool	Kqueue::addEvent(int fd, void *udata, EventType type) {
	switch (type)
	{
		case LISTEN:
		EV_SET(&this->_ev_set[0], fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, udata);
		if (kevent(this->_fd, this->_ev_set, 1, NULL, 0, NULL) == -1) {
			close(fd);
			throw (FailToControlException());
		}
		break;
		case READ:
		EV_SET(&this->_ev_set[0], fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, udata);
		if (kevent(this->_fd, this->_ev_set, 1, NULL, 0, NULL) == -1) {
			close(fd);
			throw (FailToControlException());
		}
		case WRITE:
		EV_SET(&this->_ev_set[1], fd, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, udata);
		if (kevent(this->_fd, this->_ev_set, 1, NULL, 0, NULL) == -1) {
			close(fd);
			throw (FailToControlException());
		}
		//To do: Timer 추가 해야됨 
		break;
	}
	return (true);
}
bool	Kqueue::deleteEvent(int fd, void *udata, EventType type) {
	switch (type)
	{
		case LISTEN:
		EV_SET(&this->_ev_set[0], fd, EVFILT_READ, EV_DELETE, 0, 0, udata);
		if (kevent(this->_fd, this->_ev_set, 1, NULL, 0, NULL) == -1) {
			close(fd);
			throw (FailToControlException());
		}
		break;
		case READ:
		EV_SET(&this->_ev_set[0], fd, EVFILT_READ, EV_DELETE, 0, 0, udata);
		if (kevent(this->_fd, this->_ev_set, 1, NULL, 0, NULL) == -1) {
			close(fd);
			throw (FailToControlException());
		}
		case WRITE:
		EV_SET(&this->_ev_set[1], fd, EVFILT_WRITE, EV_DELETE, 0, 0, udata);
		if (kevent(this->_fd, this->_ev_set, 1, NULL, 0, NULL) == -1) {
			close(fd);
			throw (FailToControlException());
		}
		//To do: Timer 추가 해야됨 
		break;
	}
	return (true);
}

// Exception
const char	*Kqueue::FailToCreateException::what(void) const throw() { return ("Kqueue: Fail to create"); }
const char	*Kqueue::FailToControlException::what(void) const throw() { return ("Kqueue: Fail to control"); }
const char	*Kqueue::FailToGetEventException::what(void) const throw() { return ("Kqueue: Fail to get event"); }
const char	*Kqueue::TimeoutException::what(void) const throw() { return ("Kqueue: Timeout"); }