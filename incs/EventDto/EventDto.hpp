#ifndef EVENTDTO_HPP
# define EVENTDTO_HPP

# include "../VirtualServerMap/VirtualServerMap.hpp"

class EventDto {
	private : 
		int _fd;
		const VirtualServerMap::TargetMap  *_TargetMap;
	public :
		EventDto(int fd, VirtualServerMap::TargetMap *getTargetMap);
		EventDto(int fd);
		int getFd() const;
		const VirtualServerMap::TargetMap *getTargetMap() const;
};

#endif