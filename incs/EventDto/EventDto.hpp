#ifndef EVENTDTO_HPP
# define EVENTDTO_HPP

# include "../Server/VirtualServer.hpp"
# include "../Server/PhysicalServer.hpp"
class EventDto {
	private: 
		int 									_fd;
		ft::shared_ptr<PhysicalServer>			_physical_server;

	public:
		EventDto(void);
		EventDto(int fd);
		EventDto(int fd, ft::shared_ptr<PhysicalServer> physical_server);
		EventDto(const EventDto &ref);
		~EventDto(void);
		EventDto &operator=(const EventDto &rhs);
	
	public:
		int 									getFd() const;
		ft::shared_ptr<PhysicalServer>			getPhysicalServer() const;
};

#endif