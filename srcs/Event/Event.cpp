#include "../../incs/Event/Event.hpp"
#include <new>

Event::Event(void): _fd(ft::shared_ptr<FileDescriptor>(NULL)), _event_handler(NULL) {}
Event::Event(int fd, EventHandler *event_handler): _fd(ft::shared_ptr<FileDescriptor>(new FileDescriptor(fd))), _event_handler(event_handler) {}
Event::~Event(void) {}

ft::shared_ptr<FileDescriptor>	Event::getFileDescriptor(void) { return (this->_fd); }
int	Event::getFd(void) const { return (this->_fd->getFd()); }

// To do: check Event Handler 랑 EventFactory구현 물 채워야 되는지
EventHandler::EventHandler(void) {}
EventHandler::~EventHandler(void) {}

EventFactory::EventFactory(void) {}
EventFactory::~EventFactory(void) {}

//Exception
const char	*Event::FailToOnboardException::what(void) const throw() { return ("Fail to onboard"); }
const char	*Event::FailToOffboardException::what(void) const throw() { return ("Fail to offboard"); }