#include "../../../incs/Event/ReadEvent/ReadEvent.hpp"
#include "../../../incs/Event/BufReadHandler.hpp"
#include "../../../incs/FtUtil/ft.hpp"

// ReadEvent::ReadEvent(int fd, EventHandler *read_event_handler): 
// Event(fd, read_event_handler), _buffer(ft::bufSize) {}
ReadEvent::ReadEvent(ft::shared_ptr<FileDescriptor> fd, 
ReadEventHandler *readEventHandler): 
Event(fd, readEventHandler), _buffer(ft::bufSize) {}
ReadEvent::~ReadEvent(void) {}

const std::vector<char>	&ReadEvent::getBuffer(void) { return (this->_buffer); }
std::vector<char>	&ReadEvent::getBufferRef(void) { return (this->_buffer); }