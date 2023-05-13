#include "../../incs/event/ReadEvent.hpp"
#include <new>
#include <string>

ReadEvent::ReadEvent(void): Event() {}
ReadEvent::ReadEvent(int fd): Event(fd) {}
ReadEvent::ReadEvent(const ReadEvent &ref): Event(ref) {}
ReadEvent::~ReadEvent(void) {}
ReadEvent	&ReadEvent::operator=(const ReadEvent &rhs) {
	if (this != &rhs) {
		this->~ReadEvent();
		new (this) ReadEvent(rhs);
	}
	return (*this);
}

// Getters
const std::vector<char>	&ReadEvent::getBuffer(void) const { return (this->_buffer); }

// Utils
bool	ReadEvent::append(const char *buf) {
	std::string	str(buf);

	this->_buffer.insert(this->_buffer.end(), str.begin(), str.end());
	return (true);
}