#include "../../incs/event/TimeEvent.hpp"
#include <new>

// To do: implement TimeEvent

TimeEvent::TimeEvent(void): Event() {}
TimeEvent::TimeEvent(const TimeEvent &ref): Event(ref) {}
TimeEvent::~TimeEvent(void) {}
TimeEvent	&TimeEvent::operator=(const TimeEvent &rhs) {
	if (this != &rhs) {
		this->~TimeEvent();
		new (this) TimeEvent(rhs);
	}
	return (*this);
}