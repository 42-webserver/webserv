#include "../../../incs/Event/EventBase/Event.hpp"

Event::Event(ft::shared_ptr<FileDescriptor> fd, 
EventHandler* eventHandler): _fd(fd), _event_handler(ft::shared_ptr<EventHandler>(eventHandler)) {}
Event::~Event(void) {}

ft::shared_ptr<FileDescriptor>	Event::getFileDescriptor(void) { return (this->_fd); }
int	Event::getFd(void) const { return (this->_fd->getFd()); }

//Exception
const char	*Event::FailToOnboardException::what(void) const throw() { return ("Fail to onboard"); }
const char	*Event::FailToOffboardException::what(void) const throw() { return ("Fail to offboard"); }