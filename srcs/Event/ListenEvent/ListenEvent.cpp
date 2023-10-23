#include "../../../incs/Event/ListenEvent/ListenEvent.hpp"


ListenEvent::ListenEvent(ft::shared_ptr<Channel> channel, ft::shared_ptr<VirtualServerManager> virtual_server_manager):
Event(new ListenEventHandler()), SingleStreamable(channel), _virtualServerManager(virtual_server_manager)
	{}

ListenEvent::~ListenEvent(void) {}

void	ListenEvent::callEventHandler(void) { this->_event_handler->handleEvent(*this); }
void	ListenEvent::onboardQueue(void){
	EventQueue	&event_queue = EventQueue::getInstance();
	int			fd = this->getFd();
	Event		*event = this;

	Logger::getInstance().info("onboard Listen Event");
	
	try {
		this->getChannel().get()->setNonBlocking();
	} catch (const std::exception &e) {
		Logger::getInstance().error(e.what());
		Logger::getInstance().error("Fail to accept client");
		throw (Event::FailToOnboardException());
		return ;
	}

	EV_SET(
		event_queue.getEventSetElementPtr(),
		fd,
		EVFILT_READ,
		EV_ADD | EV_ENABLE,
		0,
		0,
		event
	);

 	struct sockaddr_in localAddress;
    socklen_t addressLength = sizeof(localAddress);
    if (getsockname(fd, (struct sockaddr*)&localAddress, &addressLength) == -1) {
        perror("getsockname");
		throw std::runtime_error("getsockname");
    }
	int localPort = ntohs(localAddress.sin_port);
	ft::shared_ptr<VirtualServerManager> vsm = this->getVirtualServerManager();
	vsm->setPort(localPort);
	
	if (kevent(event_queue.getEventQueueFd(), event_queue.getEventSet(), 
	1, NULL, 0, NULL) == -1) {
		perror("kevent");
		throw (KqueueError());
	}
}
void	ListenEvent::offboardQueue(void){
	EventQueue &event_queue = EventQueue::getInstance();

	Logger::getInstance().info("Remove Listen Event");
	EV_SET(
		event_queue.getEventSetElementPtr(),
		this->getFd(),
		EVFILT_READ,
		EV_DELETE,
		0,
		0,
		this
	);


	if (kevent(event_queue.getEventQueueFd(), event_queue.getEventSet(), 1, NULL, 0, NULL) == -1) {
		throw (KqueueError());
	}
	delete this;
}

ft::shared_ptr<VirtualServerManager>	ListenEvent::getVirtualServerManager(void) const { return (this->_virtualServerManager); }
// Exception
const char	*ListenEvent::FailToAcceptException::what(void) const throw() { return ("ListenEvent: Fail to accept"); }
const char	*ListenEvent::FailToControlException::what(void) const throw() { return ("ListenEvent: Fail to control"); }
