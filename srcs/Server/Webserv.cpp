#include "../../incs/Server/Webserv.hpp"
#include <unistd.h>
#include "../../libs/unique_ptr/unique_ptr.hpp"

Webserv::Webserv(void): _physicalServerManager() {}
//Refactoring::daegulee- construcotr -> Method

Webserv::~Webserv(void) {}
Webserv	&Webserv::operator=(const Webserv &rhs) {
	if (this != &rhs) {
		this->~Webserv();
		new (this) Webserv(rhs);
	}
	return (*this);
}

void	Webserv::build(const Config &config) throw(std::exception) {
	// Build physical servers
	try {
		_physicalServerManager.run(config);
	} catch (const std::exception &e) {
		Logger::getInstance().error(e.what());
		throw (FailToConstructException());
	}

	// EventQueue	&event_queue = EventQueue::getInstance();
	// Comment Not to Change Relation, Add Method for All Listening Event Build
	
	// for (int i = 0; i < _physicalServerManager.getVirtualServerManagerCount(); i++) {
	// 	//To do: 1. EventFactroy.createEvent(Listen, )
	// 	EventFactory& eventFactory = ListenEventFactory::getInstance();
	// 	// event_queue.pushEvent(eventFactory.createEvent(_physicalServerManager.getCurrentVirtualServerManager()));
	// }
}

bool	Webserv::run(void) throw(std::exception) {
	EventQueue	&event_queue = EventQueue::getInstance();
	int			event_length;
	Event		*event_data;

	// Run server
	while (true) {
		// Polling event
		try {
			event_length = event_queue.pullEvents();
		} catch (const std::exception &e) {
			Logger::getInstance().error(e.what());
			throw (FailToRunException());
		}

		// Handle event
		for (int i=0; i<event_length; i++) {
			event_data = event_queue.getEventData(i);
			event_data->callEventHandler();
		}
	}
	return (true);
}

const char	*Webserv::TooManyServersException::what() const throw() { return ("Webserv: Too many server_configs"); }
const char	*Webserv::FailToConstructException::what() const throw() { return ("Webserv: Fail to construct"); }
const char	*Webserv::FailToRunException::what() const throw() { return ("Webserv: Fail to run"); }

// getter 사라져서 다시 구현 필요
// std::ostream	&operator<<(std::ostream &os, const Webserv &webserv) {
// 	const Webserv::PhysicalServerMap	&physical_server_map = webserv.getPhysicalServerMap();

// 	for (Webserv::PhysicalServerMap::const_iterator it=physical_server_map.begin(); it!=physical_server_map.end(); ++it) {
// 		os << "Physical Server: " << it->first.first << " " << it->first.second << '\n';
// 		os << *(it->second) << '\n';
// 	}	
// 	return (os);
// }