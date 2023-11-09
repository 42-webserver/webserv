#include "../../../incs/Event/ListenEvent/ListenEventHandler.hpp"
#include "../../../incs/Event/EventQueue/EventQueue.hpp"
#include "../../../incs/Event/EventBase/EventFactory.hpp"
#include "../../../incs/Event/ListenEvent/ListenEvent.hpp"
#include "../../../incs/Event/EventDto/EventDto.hpp"
#include <Channel/Socket.hpp>

ListenEventHandler::ListenEventHandler() {};
ListenEventHandler::~ListenEventHandler() {};

std::pair<int, std::string>		ListenEventHandler::connectClient(int server_fd) const{
	int	client_fd;
	struct sockaddr_in address;
	socklen_t address_len = sizeof(address);
	if ((client_fd = ::accept(server_fd, 
	(struct sockaddr *) &address, &address_len)) == -1) {
		Logger::getInstance().error("Fail to accept client");
		throw (FailToAcceptException());
	}
	return std::make_pair(client_fd, Socket::custom_inet_ntoa(address.sin_addr));
}

void	ListenEventHandler::handleEvent(Event &event) {
	
	Logger		&log = Logger::getInstance();
	std::pair<int, std::string> client;

	try {
		client = connectClient(static_cast<ListenEvent &>(event).getFd());
		// log.info("Client connected");
	} catch (const std::exception &e) {
		log.error(e.what());
		return ;
	}

	EventFactory &factory = EventFactory::getInstance();
	try {
		ListenEvent *listenEvent =static_cast<ListenEvent *>(&event);
		// EventDto event_dto(ft::static_pointer_cast<Channel>(ft::make_shared<Socket>(client_fd)), 
		// listenEvent->getVirtualServerManager());
		//Todo: socket New Constructor
		EventDto event_dto(ft::shared_ptr<Channel>(new Socket(client.first, client.second)),
		listenEvent->getVirtualServerManager());

		Event *readEventFromClient = factory.createEvent(ft::READ_EVENT_FROM_CLIENT, event_dto);
		readEventFromClient->onboardQueue();
	} catch (const std::exception &e) {
		log.error(e.what());
	}
}
const char	*ListenEventHandler::FailToAcceptException::what(void) const throw() { return ("ListenEventHandler: Fail to accept"); }
const char	*ListenEventHandler::FailToControlException::what(void) const throw() { return ("ListenEventHandler: Fail to control"); }