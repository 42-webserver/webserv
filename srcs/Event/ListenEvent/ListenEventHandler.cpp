#include "../../../incs/Event/ListenEvent/ListenEventHandler.hpp"

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

	// EventFactory &factory = ReadEventClientFactory::getInstance();
	// try {
	// 	EventQueue &event_queue = EventQueue::getInstance();
	// 	ListenEvent *listenEvent = static_cast<ListenEvent *>(&event);

	// 	EventDto event_dto(client_fd, listenEvent->getPhysicalServer());
	// 	event_queue.pushEvent(factory.createEvent(event_dto));
	// } catch (const std::exception &e) {
	// 	log.error(e.what());
	// 	//check: 이렇게 하는게 맞을지 생각
	// }
}
const char	*ListenEventHandler::FailToAcceptException::what(void) const throw() { return ("ListenEventHandler: Fail to accept"); }
const char	*ListenEventHandler::FailToControlException::what(void) const throw() { return ("ListenEventHandler: Fail to control"); }