#include "../../../incs/Event/ListenEvent/ListenEventHandler.hpp"
#include "../../../incs/Event/EventQueue/EventQueue.hpp"
#include "../../../incs/Event/EventBase/EventFactory.hpp"
#include "../../../incs/Event/ListenEvent/ListenEvent.hpp"
#include "../../../incs/Event/EventDto/EventDto.hpp"
ListenEventHandler::ListenEventHandler() {};
ListenEventHandler::~ListenEventHandler() {};

int		ListenEventHandler::connectClient(int server_fd) const throw(std::exception) {
	int	client_fd;
	
	if ((client_fd = ::accept(server_fd, (struct sockaddr *) NULL, NULL)) == -1) {
		Logger::getInstance().error("Fail to accept client");
		throw (FailToAcceptException());
	}
	return (client_fd);
}

void	ListenEventHandler::handleEvent(Event &event) throw (std::exception) {
	Logger		&log = Logger::getInstance();
	int 		client_fd;

	try {
		client_fd = connectClient(event.getFd());
		log.info("Client connected");
	} catch (const std::exception &e) {
		log.error(e.what());
		return ;
	}
	EventFactory &factory = EventFactory::getInstance();
	try {
		ListenEvent *listenEvent =static_cast<ListenEvent *>(&event);
		EventDto event_dto(ft::static_pointer_cast<Channel>(ft::make_shared<Socket>(client_fd)), 
		listenEvent->getVirtualServerManager());

		Event *readEventFromClient = factory.createEvent(ft::READ_EVENT_FROM_CLIENT, event_dto);
		readEventFromClient->onboardQueue();
	} catch (const std::exception &e) {
		log.error(e.what());
	}
}
const char	*ListenEventHandler::FailToAcceptException::what(void) const throw() { return ("ListenEventHandler: Fail to accept"); }
const char	*ListenEventHandler::FailToControlException::what(void) const throw() { return ("ListenEventHandler: Fail to control"); }