#ifndef READEVENTFROMCLIENT_HPP
# define READEVENTFROMCLIENT_HPP

# include "ReadEvent.hpp"

class ReadEventFromClient: public ReadEvent {
	public:
		ReadEventFromClient(ft::shared_ptr<Channel> channel, 
			ft::shared_ptr<VirtualServerManager> virtualServerManager);
		virtual	~ReadEventFromClient(void);

	public:
		const ft::shared_ptr<VirtualServerManager>	&getVirtualServerManger(void) const;

	public:
		virtual void	callEventHandler(void);
		virtual void	onboardQueue(void);
		virtual void	offboardQueue(void);
	
	private:
		ft::shared_ptr<VirtualServerManager>	_virtualServerManager;

};
#endif