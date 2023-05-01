#include "../../incs/kqueue/Kqueue.hpp"
#include <new>
#include <stddef.h>
#include <unistd.h>

// Constructor & Destructor
Kqueue::Kqueue(void) {}
Kqueue::Kqueue(int server_fd) {
	if ((this->_kq_fd = kqueue()) == -1) {
		throw (Kqueue::FailToCreateException());
	}
	this->setEvent(server_fd, EVFILT_READ, EV_ADD | EV_ENABLE, NULL);
	this->setEvent(server_fd, EVFILT_WRITE, EV_ADD | EV_ENABLE, NULL);
	if (kevent(this->_kq_fd, this->_ev_set, 2, NULL, 0, NULL) == -1) {
		throw (Kqueue::FailToControlException());
	}
}
Kqueue::~Kqueue(void) { close(this->_kq_fd); }
Kqueue::Kqueue(const Kqueue &ref): _kq_fd(ref._kq_fd) {
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

// Setters
bool	Kqueue::setEvent(int fd, int filter, int flags, void *udata) throw(std::exception) {
	if (filter == EVFILT_READ) {
		EV_SET(&this->_ev_set[0], fd, filter, flags, 0, 0, udata);
	} else if (filter == EVFILT_WRITE) {
		EV_SET(&this->_ev_set[1], fd, filter, flags, 0, 0, udata);
	} else {
		throw (Kqueue::InvalidParameterException());
	}
	return (true);
}

// Util
int	Kqueue::getEventCount(void) {
	return (kevent(this->_kq_fd, NULL, 0, this->_ev_list, MAX_EVENTS, NULL));
}

int	Kqueue::getEventFd(int idx) const throw(std::exception) {
	if (idx < 0 || MAX_EVENTS <= idx) {
		throw (Kqueue::OutOfRangeException());
	}
	return (this->_ev_list[idx].ident);
}

// Exception
const char	*Kqueue::FailToCreateException::what(void) const throw() { return ("Fail to create"); }
const char	*Kqueue::FailToControlException::what(void) const throw() { return ("Fail to control"); }
const char	*Kqueue::InvalidParameterException::what(void) const throw() { return ("Invalid parameter"); }
const char	*Kqueue::FailToGetEventCountException::what(void) const throw() { return ("Fail to get event count"); }
const char	*Kqueue::TimeoutException::what(void) const throw() { return ("Timeout"); }
const char	*Kqueue::OutOfRangeException::what(void) const throw() { return ("Out of range"); }