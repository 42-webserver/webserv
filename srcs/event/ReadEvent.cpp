#include "../../incs/event/ReadEvent.hpp"
#include <new>
#include <string>

// READ EVENT start!
ReadEvent::ReadEvent(int fd, EventHandler *ReadEventHandler):
Event(fd, ReadEventHandler), _buffer(ReadEvent::BUF_SIZE) {}
ReadEvent::~ReadEvent(void) {}

// To do: check Read Buffer need 
// const std::vector<char>	&ReadEvent::getBuffer(void) const { return (this->_buffer); }

ReadEventHandler::~ReadEventHandler() {
    // Cleanup code here
}


// READ EVENT End!

// Read EvClient start!
//	To do : check ReadEvClient

ReadEvClient::ReadEvClient(int fd, EventHandler *ReadEvClientHandler) :  
ReadEvent(fd, ReadEvClientHandler) {}

ReadEvClient::~ReadEvClient() {
}

void ReadEvClient::callEventHandler() {
	this->_eventHandler->handleEvent(*this);
}

void ReadEvClient::onboardQueue() throw (std::exception) {
	//to do!
}

void ReadEvClient::offboardQueue() throw (std::exception) {
	//to do!
}

// Read EvClient end!

// Read EvClientHandler start!
// To do : implement ReadEvClientHandler
ReadEvClientHandler::ReadEvClientHandler() {}
void ReadEvClientHandler::handleEvent(Event &event) {
	//test용
	event.callEventHandler();
	std::cout << "hi?\n";
	//
}
ReadEvClientHandler::~ReadEvClientHandler() {
}
// Read EvClientHandler

// Read EvClientFactory start!