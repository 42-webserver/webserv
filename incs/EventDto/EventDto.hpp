#ifndef EVENTDTO_HPP
# define EVENTDTO_HPP

# include "../Server/VirtualServerManager.hpp"

class EventDto {
	private: 
		int 									_fd;
		ft::shared_ptr<VirtualServerManager>	_physical_server;

	public:
		EventDto(void);
		EventDto(int fd);
		EventDto(int fd, ft::shared_ptr<VirtualServerManager> physical_server);
		EventDto(const EventDto &ref);
		~EventDto(void);
		EventDto &operator=(const EventDto &rhs);
	
	public:
		int 									getFd() const;
		ft::shared_ptr<VirtualServerManager>	getPhysicalServer() const;
};

#endif