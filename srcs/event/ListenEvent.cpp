#include "../../incs/event/ListenEvent.hpp"
#include <new>
#include <unistd.h>
#include <fcntl.h>
#include "../../incs/log/logger.hpp"
ListenEvent::ListenEvent(int fd, EventHandler *ReadEventHandler, 
ft::shared_ptr<Kqueue> kqueue): Event(fd, ReadEventHandler, kqueue) {}
ListenEvent::~ListenEvent(void) {}
void ListenEvent::callEventHandler(){
	this->_eventHandler->handleEvent(*this);
}

// To do: 
// Listen Event Handler 
ListenEvHandler::ListenEvHandler() {};
ListenEvHandler::~ListenEvHandler() {};
int ListenEvHandler::connectClient(int SocketFd) const throw(std::exception) {
	int					client_fd;
	struct sockaddr_in	client_addr;
	socklen_t			client_addr_len;
	int					flags;
	Logger::getInstance().info("Try to accept client");

	//For Debug
	// Set the socket to non-blocking mode before calling accept
	flags = fcntl(SocketFd, F_GETFL, 0);
	if (fcntl(SocketFd, F_SETFL, flags | O_NONBLOCK) == -1) {
		Logger::getInstance().info("Fail to control socket");
		throw (FailToControlException());
	}
	//


	
	client_addr_len = sizeof(client_addr);
	if ((client_fd = ::accept(SocketFd, (struct sockaddr *)&client_addr, &client_addr_len)) == -1) {
		Logger::getInstance().info("Fail to accept client");
		throw (FailToAcceptException());
	}

	// Set the client socket to non-blocking mode after accept
	if ((flags = fcntl(client_fd, F_GETFL, 0)) == -1) {
		Logger::getInstance().info("Fail to control client");
		close(client_fd);
		throw (FailToControlException());
	}
	if (fcntl(client_fd, F_SETFL, flags | O_NONBLOCK) == -1) {
		Logger::getInstance().info("Fail to control client");
		close(client_fd);
		throw (FailToControlException());
	}

	Logger::getInstance().info("Success to accept client");
	return (client_fd);	
}

void ListenEvHandler::handleEvent(Event &event) throw (std::exception) {
	int client_fd = connectClient(event.getFd());
	Logger &log = Logger::getInstance();
	if (client_fd == -1) {
		log.error("Fail to accept client");
		return ;
	}

	else {
		log.info("Success to accept client");
	}

	ListenEvFactory &factory = ListenEvFactory::getInstance();
	try {
		// To do: Listen Event 로 수정해야됨

		event.getKqueue()->addEvent(client_fd, factory.createEvent(client_fd, event.getKqueue()),
		READ);
	} catch (const std::exception &e) {
		log.error(e.what());

		//check: 이렇게 하는게 맞을지 생각 
		close(client_fd);
	}
}
// void ListenEvHandler::handleEvent(Event &event) throw (std::exception) {
// 	int					client_fd;
// 	struct sockaddr_in	client_addr;
// 	socklen_t			client_addr_len;
// 	int					flags;
// }

// Method 구현 : 꼭 vtable신경
// Exception
const char	*ListenEvent::FailToAcceptException::what(void) const throw() { return ("ListenEvent: Fail to accept"); }
const char	*ListenEvent::FailToControlException::what(void) const throw() { return ("ListenEvent: Fail to control"); }