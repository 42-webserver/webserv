# include "../../../incs/Event/EventDto/EventDto.hpp"

EventDto::EventDto(void): _channel(ft::shared_ptr<Channel>(NULL)), _virtualServerManager(ft::shared_ptr<VirtualServerManager>(NULL)) {}

EventDto::EventDto(ft::shared_ptr<Channel> channel, 
ft::shared_ptr<VirtualServerManager> virtualServerManager,
std::vector<char> *content) :
_channel(channel), _virtualServerManager(virtualServerManager),_content(content) {}

EventDto::EventDto(ft::shared_ptr<IoReadAndWriteBuffer> buffer,
const std::string &path, std::string mode,
std::vector<char>*content): _buffer(buffer), _content(content) , _path(path), _mode(mode) {}

EventDto::EventDto(std::vector<char> *content,
const std::string &path, std::string mode):  _content(content) ,_path(path), _mode(mode) {}

EventDto::EventDto(ft::shared_ptr<Client> client,
ft::shared_ptr<Channel> channel,
std::vector<char> *content): _channel(channel), _client(client), _content(content) {}

EventDto::EventDto(ft::shared_ptr<Channel> channel,
ft::shared_ptr<VirtualServerManager> virtualServerManager,
ft::shared_ptr<Client> client,
std::vector<char> *content): _channel(channel), _virtualServerManager(virtualServerManager), _client(client), _content(content) {}
EventDto::~EventDto(void) {}



const ft::shared_ptr<Channel> &EventDto::getChannel() const { return (this->_channel); }
const ft::shared_ptr<VirtualServerManager> &EventDto::getVirtualServerManager() const { return (this->_virtualServerManager); }
const ft::shared_ptr<IoReadAndWriteBuffer> &EventDto::getBuffer() const { return (this->_buffer); }
const std::string &EventDto::getPath() const { return (this->_path); }
const std::string &EventDto::getMode() const { return (this->_mode); }
std::vector<char> &EventDto::getContent() const { return (*this->_content); }
const ft::shared_ptr<Client> &EventDto::getClient() const { return (this->_client); }