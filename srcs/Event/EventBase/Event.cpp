#include "../../../incs/Event/EventBase/Event.hpp"

Event::Event(void): _fd(ft::shared_ptr<FileDescriptor>(NULL)), _event_handler(NULL) {}
Event::Event(int fd, EventHandler *event_handler): _fd(ft::shared_ptr<FileDescriptor>(new FileDescriptor(fd))), _event_handler(event_handler) {}
Event::~Event(void) {}

ft::shared_ptr<FileDescriptor>	Event::getFileDescriptor(void) { return (this->_fd); }
int	Event::getFd(void) const { return (this->_fd->getFd()); }

//Exception
const char	*Event::FailToOnboardException::what(void) const throw() { return ("Fail to onboard"); }
const char	*Event::FailToOffboardException::what(void) const throw() { return ("Fail to offboard"); }