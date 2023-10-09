#include "../../incs/Channel/Pipe.hpp"

Pipe::Pipe(void) {}
Pipe::~Pipe(void) {
	this->destroyChannelFd();
}
Pipe::Pipe(const Pipe &ref) { *this = ref; }
Pipe	&Pipe::operator=(const Pipe &rhs) {
	if (this != &rhs) {
		this->setChannelFd(rhs.getFd());
	}
	return (*this);
}
void Pipe::inJectChannelFd(int fd) {
	//Todo:여기에 pipe콜이 있을 수도 있음( method로 뺼확률 높음 )
	this->setChannelFd(fd);
}


