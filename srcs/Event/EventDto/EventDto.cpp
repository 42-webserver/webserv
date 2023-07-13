# include "../../../incs/Event/EventDto/EventDto.hpp"

EventDto::EventDto(void): _fd(ft::shared_ptr<FileDescriptor>(NULL)), _virtualServerManager(ft::shared_ptr<VirtualServerManager>(NULL)) {}
EventDto::EventDto(ft::shared_ptr<FileDescriptor> fd, 
ft::shared_ptr<VirtualServerManager> virtualServerManager = ft::shared_ptr<VirtualServerManager>(),
ft::shared_ptr<HttpRequest> httpRequest = ft::shared_ptr<HttpRequest>()): 
_fd(fd), _virtualServerManager(virtualServerManager), _httpRequest(httpRequest) {}

EventDto::~EventDto(void) {}


const ft::shared_ptr<FileDescriptor> &EventDto::getFd() const { return (this->_fd); }
const ft::shared_ptr<VirtualServerManager> &EventDto::getVirtualServerManager() const { return (this->_virtualServerManager); }
const ft::shared_ptr<HttpRequest> &EventDto::getHttpRequest() const { return (this->_httpRequest); }