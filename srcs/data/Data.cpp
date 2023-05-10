#include "../../incs/data/Data.hpp"
#include <new>

Data::Data(void): _socket(ft::shared_ptr<Socket>()) {}
Data::Data(int fd): _socket(ft::shared_ptr<Socket>(new Socket(fd))) {}
Data::Data(const Data &ref): _socket(ref._socket) {}
Data::~Data(void) {}
Data	&Data::operator=(const Data &rhs) {
	if (this != &rhs) {
		this->~Data();
		new (this) Data(rhs);
	}
	return (*this);
}

const ft::shared_ptr<Socket>	&Data::getSocket(void) const { return (this->_socket); }