#include <Channel/DualStreamable.hpp> 

DualStreamable::DualStreamable(ft::shared_ptr<CgiChannel> cgiChannel): _cgiChannel(cgiChannel) {}
DualStreamable::~DualStreamable(void) {}
// void DualStreamable::buildCgiChannel(void) { this->_cgiChannel->build(); }
int DualStreamable::getDualFd(e_pipe pipe) const { return this->_cgiChannel->getFd(pipe); }
ft::shared_ptr<CgiChannel> DualStreamable::getCgiChannel(void) const { return this->_cgiChannel; }