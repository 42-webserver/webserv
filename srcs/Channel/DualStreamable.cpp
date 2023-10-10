#include <Channel/DualStreamable.hpp> 

DualStreamable::DualStreamable() : _cgiChannel(ft::shared_ptr<CgiChannel>(new CgiChannel())) {}
DualStreamable::~DualStreamable(void) {}
void DualStreamable::buildCgiChannel(void) { this->_cgiChannel->build(); }
int DualStreamable::getDualFd(CgiChannel::e_pipe pipe) const { return this->_cgiChannel->getFd(pipe); }
ft::shared_ptr<CgiChannel> DualStreamable::getCgiChannel(void) const { return this->_cgiChannel; }