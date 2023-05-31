#include "../../incs/event/ListenEvent.hpp"
#include <new>
#include <unistd.h>
#include <fcntl.h>
#include "../../incs/log/logger.hpp"

ListenEvent::ListenEvent(int fd, EventHandler *ReadEventHandler): 
Event(fd, ReadEventHandler) {}
ListenEvent::~ListenEvent(void) {}
void ListenEvent::callEventHandler(){
	this->_eventHandler->handleEvent(*this);
}
void ListenEvent::onboardQueue() throw (std::exception) {
	EventQueue &EvQueue = EventQueue::getInstance();

	Logger::getInstance().info("onboard Listen Event");

	int listenfd = this->getConnectionFd();
	void *Event = this;

	EV_SET(EvQueue.getEvSetElementPtr(READ_SET), listenfd,
	EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, Event);
	if (kevent(EvQueue.getEvQueFd(), EvQueue.getEventSet(), 1, NULL, 0, NULL) == -1) {
		perror("kevent");
		throw (FailToControlException());
	}
}

void ListenEvent::offboardQueue() throw (std::exception) {
	EventQueue &EvQueue = EventQueue::getInstance();

	Logger::getInstance().info("Remove Listen Event");
	EV_SET(EvQueue.getEvSetElementPtr(READ_SET), this->getConnectionFd(),
	EVFILT_READ, EV_DELETE, 0, 0, this);
	if (kevent(EvQueue.getEvQueFd(), EvQueue.getEventSet(), 1, NULL, 0, NULL) == -1) {
		perror("kevent");
		throw (FailToControlException());
	}
}
// To do: 
// Listen Event Handler 
ListenEvHandler::ListenEvHandler() {};
ListenEvHandler::~ListenEvHandler() {};
int ListenEvHandler::connectClient(int PassiveSocketFd) const throw(std::exception) {
	int					client_fd;
	int					flags;
	
	if ((client_fd = ::accept(PassiveSocketFd, (struct sockaddr *)NULL, NULL)) == -1) {
		Logger::getInstance().error("Fail to accept client");
		throw (FailToAcceptException());
		return (-1);
	}

	// Set the client socket to non-blocking mode after accept
	if ((flags = fcntl(client_fd, F_GETFL, 0)) == -1) {
		Logger::getInstance().error("Fail to control client");
		close(client_fd);
		throw (FailToControlException());
	}
	if (fcntl(client_fd, F_SETFL, flags | O_NONBLOCK) == -1) {
		Logger::getInstance().error("Fail to control client");
		close(client_fd);
		throw (FailToControlException());
	}

	return (client_fd);	
}

void ListenEvHandler::handleEvent(Event &event) throw (std::exception) {
	static int i = 1;

	std::cout << i++ <<'\n';
	Logger &log = Logger::getInstance();
	int client_fd;
	try {
		client_fd = connectClient(event.getConnectionFd());
		if (client_fd != -1)
			log.info("Client connected");
	} catch (const std::exception &e) {
		log.error(e.what());
		return ;
	}

	//to do: readEvClientFactory
	// readEvClientFactory &factory = ListenEvFactory::getInstance();
	// try {
	// 	// To do: 싱글톤 호출로 바꿈 
	// 	EventQueue &EvQueue = EventQueue::getInstance();
	// 	EvQueue.pushEvent(factory.createEvent(client_fd));
	// } catch (const std::exception &e) {
	// 	log.error(e.what());
	// 	//check: 이렇게 하는게 맞을지 생각
	// }
}

// Method 구현 : 꼭 vtable신경
// Exception
const char	*ListenEvent::FailToAcceptException::what(void) const throw() { return ("ListenEvent: Fail to accept"); }
const char	*ListenEvent::FailToControlException::what(void) const throw() { return ("ListenEvent: Fail to control"); }