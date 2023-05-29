#include "../../incs/event/ListenEvent.hpp"
#include <new>
#include <unistd.h>
#include <fcntl.h>

ListenEvent::ListenEvent(int fd, EventHandler *ReadEventHandler): Event(fd, ReadEventHandler) {}
ListenEvent::~ListenEvent(void) {}
void ListenEvent::callEventHandler(){
	this->_eventHandler->handleEvent(*this);
}
// To do:: ListentEvent::accept를 Event Handler 의 매서드로 넘겨주어야 함
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

// To do: 
// Listen Event Handler 
// Method 구현 : 꼭 vtable신경

// Exception
const char	*ListenEvent::FailToAcceptException::what(void) const throw() { return ("ListenEvent: Fail to accept"); }
const char	*ListenEvent::FailToControlException::what(void) const throw() { return ("ListenEvent: Fail to control"); }