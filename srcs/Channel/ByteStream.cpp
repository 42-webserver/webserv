#include "../../incs/Channel/ByteStream.hpp"

ByteStream::ByteStream(int fd) : Channel(fd) {}
ByteStream::~ByteStream(void) {}
void ByteStream::inJectChannelFd(int fd) { this->setChannelFd(fd); }
