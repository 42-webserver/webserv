# include "../../incs/EventDto/EventDto.hpp"

EventDto::EventDto(int fd, const PhysicalServer::VirtualServerMap *TargetMap) : _fd(fd), _TargetMap(TargetMap) {}
EventDto::EventDto(int fd)  : _fd(fd), _TargetMap(NULL) {}
EventDto::~EventDto(void) {}
int EventDto::getFd() const { return _fd; }
const PhysicalServer::VirtualServerMap *EventDto::getTargetMap() const { return _TargetMap; }
