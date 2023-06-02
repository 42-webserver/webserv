#include "../../incs/Event/ListenEvent.hpp"
#include "../../incs/Log/Logger.hpp"
#include <new>
#include <unistd.h>
#include <fcntl.h>

ListenEvent::ListenEvent(int fd, EventHandler *listen_event_handler): Event(fd, listen_event_handler) {}
ListenEvent::~ListenEvent(void) {}

void	ListenEvent::callEventHandler(void) { this->_event_handler->handleEvent(*this); }
void	ListenEvent::onboardQueue(void) throw (std::exception) {
	EventQueue	&event_queue = EventQueue::getInstance();
	int			fd = this->getFd();
	void		*event = this;

	Logger::getInstance().info("onboard Listen Event");

	EV_SET(
		event_queue.getEventSetElementPtr(READ_SET),
		fd,
		EVFILT_READ,
		EV_ADD | EV_ENABLE,
		0,
		0,
		event
	);

	if (kevent(event_queue.getEventQueueFd(), event_queue.getEventSet(), 1, NULL, 0, NULL) == -1) {
		// perror("kevent");
		throw (FailToControlException());
	}
}
void	ListenEvent::offboardQueue(void) throw (std::exception) {
	EventQueue &event_queue = EventQueue::getInstance();

	Logger::getInstance().info("Remove Listen Event");
	EV_SET(
		event_queue.getEventSetElementPtr(READ_SET),
		this->getFd(),
		EVFILT_READ,
		EV_DELETE,
		0,
		0,
		this
	);

	if (kevent(event_queue.getEventQueueFd(), event_queue.getEventSet(), 1, NULL, 0, NULL) == -1) {
		// perror("kevent");
		throw (FailToControlException());
	}
}

// To do: 
// Listen Event Handler 
ListenEventHandler::ListenEventHandler(void) {};
ListenEventHandler::~ListenEventHandler(void) {};

int		ListenEventHandler::connectClient(int server_fd) const throw(std::exception) {
	int	client_fd;
	int	flags;
	
	if ((client_fd = ::accept(server_fd, (struct sockaddr *) NULL, NULL)) == -1) {
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
void	ListenEventHandler::handleEvent(Event &event) throw (std::exception) {
	static int	i = 1;
	Logger		&log = Logger::getInstance();
	int 		client_fd;

	std::cout << i++ <<'\n';
	try {
		client_fd = connectClient(event.getFd());
		if (client_fd != -1) {
			log.info("Client connected");
		}
	} catch (const std::exception &e) {
		log.error(e.what());
		return ;
	}

	//to do: readEvClientFactory
	// readEvClientFactory &factory = ListenEventFactory::getInstance();
	// try {
	// 	// To do: 싱글톤 호출로 바꿈 
	// 	EventQueue &event_queue = EventQueue::getInstance();
	// 	event_queue.pushEvent(factory.createEvent(client_fd));
	// } catch (const std::exception &e) {
	// 	log.error(e.what());
	// 	//check: 이렇게 하는게 맞을지 생각
	// }
}

ListenEventFactory::ListenEventFactory(void) {}
ListenEventFactory::~ListenEventFactory(void) {}
ListenEventFactory	&ListenEventFactory::getInstance(void) {
	static ListenEventFactory	instance;

	return (instance);
}

Event	*ListenEventFactory::createEvent(int fd) {
	EventHandler	*event_handler = new ListenEventHandler();
	ListenEvent		*event = new ListenEvent(fd, event_handler);

	return (event);
}


// Method 구현: 꼭 vtable신경
// Exception
const char	*ListenEvent::FailToAcceptException::what(void) const throw() { return ("ListenEvent: Fail to accept"); }
const char	*ListenEvent::FailToControlException::what(void) const throw() { return ("ListenEvent: Fail to control"); }
const char	*ListenEventHandler::FailToAcceptException::what(void) const throw() { return ("ListenEventHandler: Fail to accept"); }
const char	*ListenEventHandler::FailToControlException::what(void) const throw() { return ("ListenEventHandler: Fail to control"); }