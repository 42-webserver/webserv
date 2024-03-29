#ifndef READEVENTFROMCLIENT_HPP
# define READEVENTFROMCLIENT_HPP

# include "ReadEvent.hpp"
# include <Channel/SingleStreamable.hpp>
# include <Client/Client.hpp>
# include "../../../incs/Event/ReadEvent/ReadEventFromClientHandler.hpp"
# include <Event/Exception/KqueueError.hpp>

class ReadEventFromClient: public ReadEvent, public SingleStreamable{
	public:
		ReadEventFromClient(ft::shared_ptr<Channel> channel, 
			ft::shared_ptr<VirtualServerManager> virtualServerManager);
		virtual	~ReadEventFromClient(void);

	public:
		const ft::shared_ptr<VirtualServerManager>	&getVirtualServerManger(void) const;
		bool canRead(void) const;
	public:
		virtual void	callEventHandler(void);
		virtual void	onboardQueue(void);
		virtual void	offboardQueue(void);

	public:
		void			addRequest(ft::shared_ptr<HttpRequest> request);
		e_client_event_queue_state queryClientEventQueueState(void);
		bool 			isRequestEmpty(void);
		bool			isEventQueueTurnOn(e_client_event_queue_state state);
		bool			isClientDie(void);
		ft::shared_ptr<Client> getClient(void);
	private:
		ft::shared_ptr<VirtualServerManager>	_virtualServerManager;
		ft::shared_ptr<Client>					_client;
};
#endif