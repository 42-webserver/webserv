#include "../../../incs/Event/ReadEvent/ReadEvent.hpp"
#include "../../../incs/Event/BufReadHandler.hpp"
#include "../../../incs/FtUtil/ft.hpp"

// ReadEvent::ReadEvent(int fd, EventHandler *read_event_handler): 
// Event(fd, read_event_handler), _buffer(ft::bufSize) {}
ReadEvent::ReadEvent(ReadEventHandler *readEventHandler):
Event(readEventHandler){}
ReadEvent::~ReadEvent(void) {}
