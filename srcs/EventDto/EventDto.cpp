# include "../../incs/EventDto/EventDto.hpp"

EventDto::EventDto(int fd, ft::shared_ptr<PhysicalServer> physical_server) : _fd(fd), _physical_server(physical_server) {}
EventDto::EventDto(int fd)  : _fd(fd), _physical_server(NULL) {}
EventDto::~EventDto(void) {}
int EventDto::getFd() const { return _fd; }
ft::shared_ptr<PhysicalServer> EventDto::getPhysicalServer() const { return _physical_server; }