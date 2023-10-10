#include <Channel/DualStreamable.hpp> 

DualStreamable::DualStreamable(ft::shared_ptr<CgiChannel> channel) : _cgiChannel(channel) {}
DualStreamable::~DualStreamable(void) {}
int DualStreamable::getFd(CgiChannel::e_pipe pipe) const { return this->_cgiChannel->getFd(pipe); }
ft::shared_ptr<CgiChannel> DualStreamable::getCgiChannel(void) const { return this->_cgiChannel; }