# include "../../../incs/Event/EventDto/EventDto.hpp"

EventDto::EventDto(void): _channel(ft::shared_ptr<Channel>(NULL)), _virtualServerManager(ft::shared_ptr<VirtualServerManager>(NULL)) {}
EventDto::EventDto(ft::shared_ptr<Channel> channel, 
ft::shared_ptr<VirtualServerManager> virtualServerManager,
ft::shared_ptr<HttpRequest> httpRequest): 
_channel(channel), _virtualServerManager(virtualServerManager), _httpRequest(httpRequest) {}

EventDto::~EventDto(void) {}


const ft::shared_ptr<Channel> &EventDto::getChannel() const { return (this->_channel); }
const ft::shared_ptr<VirtualServerManager> &EventDto::getVirtualServerManager() const { return (this->_virtualServerManager); }
const ft::shared_ptr<HttpRequest> &EventDto::getHttpRequest() const { return (this->_httpRequest); }