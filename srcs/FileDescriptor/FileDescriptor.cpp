#include "../../incs/FileDescriptor/FileDescriptor.hpp"

FileDescriptor::FileDescriptor(void) : _fd(0) {}
FileDescriptor::FileDescriptor(int fd) : _fd(fd) {}
FileDescriptor::~FileDescriptor(void) { ::close(this->_fd);};
FileDescriptor::FileDescriptor(const FileDescriptor &ref) : _fd(ref._fd) {}
FileDescriptor	&FileDescriptor::operator=(const FileDescriptor &rhs) {
	if (this != &rhs) {
		this->~FileDescriptor();
		new (this) FileDescriptor(rhs);
	}
	return (*this);
}

void	FileDescriptor::setNonBlocking(void) throw (std::exception) {
	int flags;

	if ((flags = fcntl(this->getFd(), F_GETFL, 0)) == -1) {
		Logger::getInstance().error("Fail to control client");
		close(this->getFd());
		throw (FailToNonBlockException());
		return ;
	}

	if (fcntl(this->getFd(), F_SETFL, flags | O_NONBLOCK) == -1) {
		Logger::getInstance().error("Fail to control client");
		close(this->getFd());
		throw (FailToNonBlockException());
	}
}

int	FileDescriptor::getFd() const { return this->_fd; }
const char	*FileDescriptor::FailToNonBlockException::what(void) const throw() { return ("FileDescriptor: Fail to setNonBlocking"); }