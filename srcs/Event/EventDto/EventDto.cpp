# include "../../../incs/Event/EventDto/EventDto.hpp"

EventDto::EventDto(void): _channel(ft::shared_ptr<Channel>(NULL)), _virtualServerManager(ft::shared_ptr<VirtualServerManager>(NULL)) {}
EventDto::EventDto(ft::shared_ptr<Channel> channel, 
ft::shared_ptr<VirtualServerManager> virtualServerManager,
ft::shared_ptr<HttpRequest> httpRequest): 
_channel(channel), _virtualServerManager(virtualServerManager), _httpRequest(httpRequest) {}
EventDto::EventDto(ft::shared_ptr<IoReadAndWriteBuffer> buffer,
const std::string &path, std::string mode): _buffer(buffer), _path(path), _mode(mode) {}
EventDto::~EventDto(void) {}


const ft::shared_ptr<Channel> &EventDto::getChannel() const { return (this->_channel); }
const ft::shared_ptr<VirtualServerManager> &EventDto::getVirtualServerManager() const { return (this->_virtualServerManager); }
const ft::shared_ptr<HttpRequest> &EventDto::getHttpRequest() const { return (this->_httpRequest); }
const ft::shared_ptr<IoReadAndWriteBuffer> &EventDto::getBuffer() const { return (this->_buffer); }
const std::string &EventDto::getPath() const { return (this->_path); }
const std::string &EventDto::getMode() const { return (this->_mode); }