#include "../../incs/Server/Webserv.hpp"
#include <unistd.h>
#include "../../libs/unique_ptr/unique_ptr.hpp"

Webserv::Webserv(void): _physicalServerManager() {}
Webserv::~Webserv(void) {}
Webserv	&Webserv::operator=(const Webserv &rhs) {
	if (this != &rhs) {
		this->~Webserv();
		new (this) Webserv(rhs);
	}
	return (*this);
}

bool	Webserv::run(const Config &config) throw(std::exception) {
	try {
		this->_build(config);
		this->_physicalServerManager.run();
	} catch (const std::exception &e) {
		Logger::getInstance().error(e.what());
		throw (FailToRunException());
	}

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

bool	Webserv::_build(const Config &config) throw(std::exception) {
	try {
		this->_physicalServerManager.build(config);
	} catch (const std::exception &e) {
		Logger::getInstance().error(e.what());
		throw (FailToBuildException());
	}
}

const char	*Webserv::FailToBuildException::what() const throw() { return ("Webserv: Fail to construct"); }
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