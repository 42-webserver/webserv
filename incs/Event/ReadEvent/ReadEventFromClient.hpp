#ifndef READEVENTFROMCLIENT_HPP
# define READEVENTFROMCLIENT_HPP

# include "ReadEvent.hpp"

class ReadEventFromClient: public ReadEvent {
	public:
		ReadEventFromClient(int fd, EventHandler *readEventFromClientHandler, ft::shared_ptr<VirtualServerManager> physicalServer);
		virtual	~ReadEventFromClient(void);

	public:
		ft::shared_ptr<VirtualServerManager>	getPhysicalServer(void) const;

	public:
		virtual void	callEventHandler(void);
		virtual void	onboardQueue(void) throw (std::exception);
		virtual void	offboardQueue(void) throw (std::exception);
	
	private:
		ft::shared_ptr<VirtualServerManager>	_physical_server;

};
#endif