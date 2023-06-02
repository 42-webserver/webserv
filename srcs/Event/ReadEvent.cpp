#include "../../incs/Event/ReadEvent.hpp"
#include <new>
#include <string>

// READ EVENT start!
ReadEvent::ReadEvent(int fd, EventHandler *read_event_handler): Event(fd, read_event_handler), _buffer(ReadEvent::BUF_SIZE) {}
ReadEvent::~ReadEvent(void) {}

// To do: check Read Buffer need 
// const std::vector<char>	&ReadEvent::getBuffer(void) const { return (this->_buffer); }

ReadEventHandler::~ReadEventHandler(void) {
	// Cleanup code here
}


// READ EVENT End!

// Read EvClient start!
//	To do : check ReadEventClient

ReadEventClient::ReadEventClient(int fd, EventHandler *read_event_client_handler): ReadEvent(fd, read_event_client_handler) {}
ReadEventClient::~ReadEventClient(void) {}

void	ReadEventClient::callEventHandler(void) { this->_event_handler->handleEvent(*this); }
void	ReadEventClient::onboardQueue() throw (std::exception) {
	//to do!
}
void	ReadEventClient::offboardQueue() throw (std::exception) {
	//to do!
}

// Read EvClient end!

// Read EvClientHandler start!
// To do : implement ReadEventClientHandler
ReadEventClientHandler::ReadEventClientHandler(void) {}
void ReadEventClientHandler::handleEvent(Event &event) {
	//test용
	event.callEventHandler();
	std::cout << "hi?\n";
	//
}

ReadEventClientHandler::~ReadEventClientHandler(void) {}
// Read EvClientHandler

// Read EvClientFactory start!