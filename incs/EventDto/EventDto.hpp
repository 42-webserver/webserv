#ifndef EVENTDTO_HPP
# define EVENTDTO_HPP

# include "../Server/VirtualServer.hpp"
# include "../Server/PhysicalServer.hpp"
class EventDto {
	private : 
		int _fd;
		const PhysicalServer::VirtualServerMap  *_TargetMap;
	public :
		EventDto(int fd, const PhysicalServer::VirtualServerMap *getTargetMap);
		EventDto(int fd);
		int getFd() const;
		const PhysicalServer::VirtualServerMap *getTargetMap() const;
};

#endif