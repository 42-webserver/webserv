#include <Channel/SingleStreamable.hpp>

SingleStreamable::SingleStreamable(ft::shared_ptr<Channel> channel) : _channel(channel) {}
SingleStreamable::~SingleStreamable(void) {}
SingleStreamable::SingleStreamable(void) {}
int SingleStreamable::getFd(void) const { return this->_channel->getFd(); }
ft::shared_ptr<Channel> SingleStreamable::getChannel(void) const { return this->_channel; }