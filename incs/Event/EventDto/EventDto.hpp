#ifndef EVENTDTO_HPP
# define EVENTDTO_HPP

# include "../../Server/VirtualServerManager.hpp"
# include "../../FileDescriptor/FileDescriptor.hpp"
# include "../../Http/Request/HttpRequest.hpp"
class  EventDto {
	private: 
		ft::shared_ptr<FileDescriptor>				_fd;
		ft::shared_ptr<VirtualServerManager>	_virtualServerManager;
		ft::shared_ptr<HttpRequest>				_httpRequest;

	public:
		EventDto(ft::shared_ptr<FileDescriptor> fd, ft::shared_ptr<VirtualServerManager> virtualServerManager,
		ft::shared_ptr<HttpRequest> httpRequest);
		~EventDto(void);
	private:
		EventDto &operator=(const EventDto &rhs);
		EventDto(void);
		EventDto(const EventDto &ref);
	public:
		const ft::shared_ptr<FileDescriptor>	&getFd() const;
		const ft::shared_ptr<VirtualServerManager>	&getVirtualServerManager() const;
		const ft::shared_ptr<HttpRequest>	&getHttpRequest() const;
};

#endif