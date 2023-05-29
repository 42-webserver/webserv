#include "../../incs/event/WriteEvent.hpp"
#include <new>

// To do: implement WriteEvent

WriteEvent::WriteEvent(void): Event() {}
WriteEvent::WriteEvent(int fd): Event(fd) {}
WriteEvent::WriteEvent(const WriteEvent &ref): Event(ref) {}
WriteEvent::~WriteEvent(void) {}
WriteEvent	&WriteEvent::operator=(const WriteEvent &rhs) {
	if (this != &rhs) {
		this->~WriteEvent();
		new (this) WriteEvent(rhs);
	}
	return (*this);
}