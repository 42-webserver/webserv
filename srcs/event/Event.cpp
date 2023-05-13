#include "../../incs/event/Event.hpp"
#include <new>

Event::Event(void): _fd(0) {}
Event::Event(int fd): _fd(fd) {}
Event::Event(const Event &ref): _fd(ref._fd) {}
Event::~Event(void) {}
Event	&Event::operator=(const Event &rhs) {
	if (this != &rhs) {
		this->~Event();
		new (this) Event(rhs);
	}
	return (*this);
}

// Getters
int	Event::getFd(void) const { return (this->_fd); }