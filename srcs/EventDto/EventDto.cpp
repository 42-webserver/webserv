# include "../../incs/EventDto/EventDto.hpp"

EventDto::EventDto(int fd, VirtualServerMap::TargetMap *TargetMap) : _fd(fd), _TargetMap(TargetMap) {}
EventDto::EventDto(int fd)  : _fd(fd), _TargetMap(NULL) {}
int EventDto::getFd() const { return _fd; }
const VirtualServerMap::TargetMap *EventDto::getTargetMap() const { return _TargetMap; }
