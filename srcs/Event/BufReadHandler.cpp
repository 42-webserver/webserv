#include "../../incs/Event/BufReadHandler.hpp"

BufReadHandler::BufReadHandler(int fd, const int buf_size): _fd(fd), _buf_size(buf_size) {}
BufReadHandler::~BufReadHandler(void) {}

std::vector<char> BufReadHandler::readBuf(void) {
	std::vector<char>	buf(this->_buf_size);
	ssize_t					read_size;

	read_size = read(this->_fd, &buf[0], this->_buf_size);
	if (read_size == 0)
		return (std::vector<char>());
	else if (read_size == -1) {
		throw (BufReadHandler::FailToReadNonBlockException());
		return (std::vector<char>());
	}
	return (buf);
}

const char	*BufReadHandler::FailToReadNonBlockException::what() const throw() { return ("BufReadHandler: Fail to read non block"); }