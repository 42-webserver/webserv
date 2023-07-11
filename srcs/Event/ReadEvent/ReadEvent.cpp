#include "../../../incs/Event/ReadEvent/ReadEvent.hpp"
#include "../../../incs/Event/BufReadHandler.hpp"

ReadEvent::ReadEvent(int fd, EventHandler *read_event_handler): Event(fd, read_event_handler), _buffer(ReadEvent::BUF_SIZE) {}
ReadEvent::~ReadEvent(void) {}

