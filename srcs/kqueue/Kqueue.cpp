#include "../../incs/kqueue/Kqueue.hpp"
#include <new>
#include <stddef.h>
#include <unistd.h>
#include <iostream>
#include <errno.h>

// Constructor & Destructor
Kqueue::Kqueue(void): _fd(0) {
	memset(this->_ev_set, 0, 2 * sizeof(struct kevent));
	memset(this->_ev_list, 0, MAX_EVENTS * sizeof(struct kevent));
}
Kqueue::~Kqueue(void) { if (this->_fd) close(this->_fd); }
Kqueue::Kqueue(const Kqueue &ref): _fd(ref._fd) {
	for (int i=0; i<2; i++) {
		this->_ev_set[i] = ref._ev_set[i];
	}
	for (int i=0; i<MAX_EVENTS; i++) {
		this->_ev_list[i] = ref._ev_list[i];
	}
}
Kqueue	&Kqueue::operator=(const Kqueue &rhs) {
	if (this != &rhs) {
		this->~Kqueue();
		new (this) Kqueue(rhs);
	}
	return (*this);
}

// Util
bool	Kqueue::init(int server_fd) throw(std::exception) {
	if ((this->_fd = kqueue()) == -1) {
		throw (Kqueue::FailToCreateException());
	}
	try {
		this->addEvent(server_fd);
	} catch (const Kqueue::FailToControlException &e) {
		throw (e);
	}
	return (true);
}

int		Kqueue::length(void) {
	int	ret = kevent(this->_fd, NULL, 0, this->_ev_list, MAX_EVENTS, NULL);
	
	if (ret == -1) {
		throw (Kqueue::FailToGetEventException());
	}
	return (ret);
}

int		Kqueue::getEventFd(int idx) const throw(std::exception) {
	return (this->_ev_list[idx].ident);
}

bool	Kqueue::addEvent(int socket_fd) {
	EV_SET(&this->_ev_set[0], socket_fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
	EV_SET(&this->_ev_set[1], socket_fd, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, NULL);
	if (kevent(this->_fd, this->_ev_set, 2, NULL, 0, NULL) == -1) {
		close(socket_fd);
		throw (Kqueue::FailToControlException());
	}
	return (true);
}

bool	Kqueue::deleteEvent(int socket_fd) {
	EV_SET(&this->_ev_set[0], socket_fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
	EV_SET(&this->_ev_set[1], socket_fd, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
	if (kevent(this->_fd, this->_ev_set, 2, NULL, 0, NULL) == -1) {
		close(socket_fd);
		throw (Kqueue::FailToControlException());
	}
	close(socket_fd);
	return (true);
}

// Exception
const char	*Kqueue::FailToCreateException::what(void) const throw() { return ("Kqueue: Fail to create"); }
const char	*Kqueue::FailToControlException::what(void) const throw() { return ("Kqueue: Fail to control"); }
const char	*Kqueue::FailToGetEventException::what(void) const throw() { return ("Kqueue: Fail to get event"); }
const char	*Kqueue::TimeoutException::what(void) const throw() { return ("Kqueue: Timeout"); }