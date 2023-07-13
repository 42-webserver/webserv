#ifndef WRITEEVENTTOCLIENT_HPP
# define WRITEEVENTTOCLIENT_HPP

# include "WriteEvent.hpp"
# include "../../Http/Request/HttpRequest.hpp"

class WriteEventToClient: public WriteEvent {
	public:
		WriteEventToClient(ft::shared_ptr<FileDescriptor> fd, 
		ft::shared_ptr<VirtualServerManager> virtualServerManager,
		ft::shared_ptr<HttpRequest> httpRequest);
		virtual	~WriteEventToClient(void);

	public:
		const ft::shared_ptr<VirtualServerManager>	&getVirtualServerManger(void) const;

	public:
		virtual void	callEventHandler(void);
		virtual void	onboardQueue(void) throw (std::exception);
		virtual void	offboardQueue(void) throw (std::exception);
	
	private:
		ft::shared_ptr<VirtualServerManager>	_virtualServerManager;
		ft::shared_ptr<HttpRequest>	_httpRequest;
};

#endif 