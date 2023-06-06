#ifndef EVENTDTO_HPP
# define EVENTDTO_HPP

# include "../Server/VirtualServer.hpp"
# include "../Server/PhysicalServer.hpp"
class EventDto {
	private: 
		int 									_fd;
		const PhysicalServer::VirtualServerMap  *_TargetMap;

	public:
		EventDto(void);
		EventDto(int fd);
		EventDto(int fd, const PhysicalServer::VirtualServerMap *getTargetMap);
		EventDto(const EventDto &ref);
		~EventDto(void);
		EventDto &operator=(const EventDto &rhs);
	
	public:
		int 									getFd() const;
		const PhysicalServer::VirtualServerMap	*getTargetMap() const;
};

#endif