#ifndef EVENTDTO_HPP
# define EVENTDTO_HPP

# include "../../Server/VirtualServerManager.hpp"
# include "../../Channel/Channel.hpp"
# include "../../Http/Request/HttpRequest.hpp"
class  EventDto {
	private: 
		ft::shared_ptr<Channel>				_channel;
		ft::shared_ptr<VirtualServerManager>	_virtualServerManager;
		ft::shared_ptr<HttpRequest>				_httpRequest;

	public:
		EventDto(ft::shared_ptr<Channel> channel, 
		ft::shared_ptr<VirtualServerManager> virtualServerManager = ft::shared_ptr<VirtualServerManager>(),
		ft::shared_ptr<HttpRequest> httpRequest = ft::shared_ptr<HttpRequest>());
		~EventDto(void);
	private:
		EventDto &operator=(const EventDto &rhs);
		EventDto(void);
		EventDto(const EventDto &ref);
	public:
		const ft::shared_ptr<Channel>	&getChannel() const;
		const ft::shared_ptr<VirtualServerManager>	&getVirtualServerManager() const;
		const ft::shared_ptr<HttpRequest>	&getHttpRequest() const;
};

#endif