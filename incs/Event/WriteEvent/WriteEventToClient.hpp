#ifndef WRITEEVENTTOCLIENT_HPP
# define WRITEEVENTTOCLIENT_HPP

# include "WriteEvent.hpp"
# include "../../Http/Request/HttpRequest.hpp"
# include <Channel/SingleStreamable.hpp>
# include <Channel/DualStreamable.hpp>
#include "../../../incs/Event/WriteEvent/WriteEventToClientHandler.hpp"
#include "../../../incs/Event/EventQueue/EventQueue.hpp"

class WriteEventToClient: public WriteEvent, public SingleStreamable, public DualStreamable{
	public:
		WriteEventToClient(ft::shared_ptr<Channel> fd, 
		ft::shared_ptr<VirtualServerManager> virtualServerManager,
		ft::shared_ptr<HttpRequest> httpRequest);
		virtual	~WriteEventToClient(void);

	public:
		const ft::shared_ptr<VirtualServerManager>	&getVirtualServerManger(void) const;

	public:
		virtual void	callEventHandler(void);
		virtual void	onboardQueue(void);
		virtual void	offboardQueue(void);
	
	private:
		ft::shared_ptr<VirtualServerManager>	_virtualServerManager;
		ft::shared_ptr<HttpRequest>	_httpRequest;
};

#endif 