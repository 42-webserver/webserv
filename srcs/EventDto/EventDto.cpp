# include "../../incs/EventDto/EventDto.hpp"

EventDto::EventDto(int fd, ft::shared_ptr<VirtualServerManager> physical_server) : _fd(fd), _physical_server(physical_server) {}
EventDto::EventDto(int fd): _fd(fd), _physical_server(NULL) {}
EventDto::~EventDto(void) {}

int EventDto::getFd() const { return (this->_fd); }
ft::shared_ptr<VirtualServerManager> EventDto::getPhysicalServer() const { return (this->_physical_server); }