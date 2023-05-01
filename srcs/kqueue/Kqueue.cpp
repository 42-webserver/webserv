#include "../../incs/kqueue/Kqueue.hpp"
#include <new>
#include <stddef.h>

// Constructor & Destructor
Kqueue::Kqueue(void) {}
Kqueue::Kqueue(int server_fd) {
	if ((this->_kq_fd = kqueue()) == -1) {
		throw (Kqueue::KqueueCreationErrorException());
	}
	EV_SET(&this->_ev_set[0], server_fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
	EV_SET(&this->_ev_set[1], server_fd, EVFILT_WRITE, EV_ADD, 0, 0, NULL);
	if (kevent(this->_kq_fd, this->_ev_set, 2, NULL, 0, NULL) == -1) {
		throw (Kqueue::KqueueControlErrorException());
	}
}
Kqueue::~Kqueue(void) {}
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

// Exception
const char	*Kqueue::KqueueCreationErrorException::what(void) const throw() { return ("Kqueue creation error"); }
const char	*Kqueue::KqueueControlErrorException::what(void) const throw() { return ("Kqueue control error"); }