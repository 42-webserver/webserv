#include "../../libs/unique_ptr/unique_ptr.hpp"
#include "../../incs/EventQueue/EventQueue.hpp"
#include "../../incs/Server/Webserv.hpp"
#include "../../incs/Log/Logger.hpp"
#include <unistd.h>

Webserv::Webserv(void): _physical_server_manager() {}
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
		this->_physical_server_manager.run();
	} catch (const std::exception &e) {
		Logger::getInstance().error(e.what());
		throw (FailToRunException());
	}

	EventQueue	&event_queue = EventQueue::getInstance();
	int			event_length;
	Event		*event_data;

	while (true) {
		try {
			event_length = event_queue.pullEvents();
		} catch (const std::exception &e) {
			Logger::getInstance().error(e.what());
			throw (FailToRunException());
		}

		for (int i=0; i<event_length; i++) {
			event_data = event_queue.getEventData(i);
			event_data->callEventHandler();
		}
	}
	return (true);
}

bool	Webserv::_build(const Config &config) throw(std::exception) {
	try {
		this->_physical_server_manager.build(config);
	} catch (const std::exception &e) {
		Logger::getInstance().error(e.what());
		throw (FailToBuildException());
	}

	return (true);
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