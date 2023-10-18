#ifndef WRITEEVENTTOCLIENT_HPP
# define WRITEEVENTTOCLIENT_HPP

# include "WriteEvent.hpp"
# include "../../Http/Request/HttpRequest.hpp"
# include <Channel/SingleStreamable.hpp>
# include <Channel/DualStreamable.hpp>
#include "../../../incs/Event/WriteEvent/WriteEventToClientHandler.hpp"
#include "../../../incs/Event/EventQueue/EventQueue.hpp"
#include <Client/Client.hpp>
class WriteEventToClient: public WriteEvent, public SingleStreamable{
	public:
		WriteEventToClient(ft::shared_ptr<Channel> fd, 
		ft::shared_ptr<VirtualServerManager> virtualServerManager,
		ft::shared_ptr<Client> client);
		virtual	~WriteEventToClient(void);

	public:
		const ft::shared_ptr<VirtualServerManager>	&getVirtualServerManger(void) const;

	public:
		virtual void	callEventHandler(void);
		virtual void	onboardQueue(void);
		virtual void	offboardQueue(void);
		e_client_event_queue_state queryClientEventQueueState(void);
		ft::shared_ptr<Client> getClient(void);
	private:
		ft::shared_ptr<VirtualServerManager>	_virtualServerManager;
		ft::shared_ptr<Client>	_client;
};

#endif 