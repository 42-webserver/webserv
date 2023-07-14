#include "../../../incs/Event/WriteEvent/WriteEvent.hpp"
#include "../../../incs/FtUtil/ft.hpp"

// To do: implement WriteEvent

WriteEvent::WriteEvent(ft::shared_ptr<FileDescriptor> fd,
WriteEventHandler* writeEventHandler): Event(fd, writeEventHandler),
 _buffer(ft::bufSize) {}
WriteEvent::~WriteEvent(void) {}

const std::vector<char>	&WriteEvent::getBuffer(void) { return (this->_buffer); }
std::vector<char>	&WriteEvent::getBufferRef(void) { return (this->_buffer); }