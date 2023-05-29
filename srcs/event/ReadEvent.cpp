#include "../../incs/event/ReadEvent.hpp"
#include <new>
#include <string>

// READ EVENT
ReadEvent::ReadEvent(int fd, EventHandler *ReadEventHandler, ft::shared_ptr<Kqueue> kqueue):
Event(fd, ReadEventHandler, kqueue), _buffer(ReadEvent::BUF_SIZE) {}
ReadEvent::~ReadEvent(void) {}

// const std::vector<char>	&ReadEvent::getBuffer(void) const { return (this->_buffer); }

ReadEventHandler::~ReadEventHandler() {
    // Cleanup code here
}


// READ EVENT

// Read EvClient

ReadEvClient::ReadEvClient(int fd, EventHandler *ReadEvClientHandler, ft::shared_ptr<Kqueue> kqueue) :  
ReadEvent(fd, ReadEvClientHandler, kqueue) {}

ReadEvClient::~ReadEvClient() {
}

void ReadEvClient::callEventHandler() {
	this->_eventHandler->handleEvent(*this);
}
// Read EvClient

// Read EvClientHandler

ReadEvClientHandler::ReadEvClientHandler() {}
void ReadEvClientHandler::handleEvent(Event &event) {
	//test용
	std::cout << "hi?\n";
	//
}
ReadEvClientHandler::~ReadEvClientHandler() {
}
// Read EvClientHandler
