#include "../../incs/event/ListenEvent.hpp"
#include <new>
#include <unistd.h>
#include <fcntl.h>

ListenEvent::ListenEvent(void): Event() {}
ListenEvent::ListenEvent(int fd): Event(fd) {}
ListenEvent::ListenEvent(const ListenEvent &ref): Event(ref) {}
ListenEvent::~ListenEvent(void) {}
ListenEvent	&ListenEvent::operator=(const ListenEvent &rhs) {
	if (this != &rhs) {
		this->~ListenEvent();
		new (this) ListenEvent(rhs);
	}
	return (*this);
}

// Utils
int		ListenEvent::accept(void) const throw(std::exception) {
	int					client_fd;
	struct sockaddr_in	client_addr;
	socklen_t			client_addr_len;
	int					flags;

	client_addr_len = sizeof(client_addr);
	if ((client_fd = ::accept(this->_fd, (struct sockaddr *)&client_addr, &client_addr_len)) == -1) {
		throw (FailToAcceptException());
	}
	if ((flags = fcntl(client_fd, F_GETFL, 0)) == -1) {
		close(client_fd);
		throw (FailToControlException());
	}
	if (fcntl(client_fd, F_SETFL, flags | O_NONBLOCK) == -1) {
		close(client_fd);
		throw (FailToControlException());
	}
	return (client_fd);
}

// Exception
const char	*ListenEvent::FailToAcceptException::what(void) const throw() { return ("ListenEvent: Fail to accept"); }
const char	*ListenEvent::FailToControlException::what(void) const throw() { return ("ListenEvent: Fail to control"); }