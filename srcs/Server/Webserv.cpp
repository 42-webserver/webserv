#include "../../incs/Server/Webserv.hpp"
#include <unistd.h>
#include "../../libs/unique_ptr/unique_ptr.hpp"

Webserv::Webserv(void): _physical_server_map(PhysicalServerMap()) {}
//Refactoring::daegulee- construcotr -> Method
Webserv::Webserv(const Config &config): _physical_server_map(_initPhysicalServerMap(config)) {}
Webserv::Webserv(const Webserv &ref): _physical_server_map(ref._physical_server_map) {}
Webserv::~Webserv(void) {}
Webserv	&Webserv::operator=(const Webserv &rhs) {
	if (this != &rhs) {
		this->~Webserv();
		new (this) Webserv(rhs);
	}
	return (*this);
}

Webserv::PhysicalServerMap	Webserv::_initPhysicalServerMap(const Config &config) throw(std::exception) {
	const std::vector<Config::map>	server_configs = config.getConfigMaps();
	const ListenEventFactory		&factory = ListenEventFactory::getInstance();
	EventQueue 						&event_queue = EventQueue::getInstance();
	PhysicalServerMap				ret;

	//Todo ::  implement feature for Update physicalServerMap ::

	if (MAX_SERVERS <= server_configs.size()) {
		throw (TooManyServersException());
	}

	for (size_t i=0; i<server_configs.size(); ++i) {
		std::string						host;
		int								port;
		ft::shared_ptr<PhysicalServer>	physical_server;

		try {
			host = _initHost(server_configs[i].at(Config::KEYS[0])[0]);
			port = _initPort(server_configs[i].at(Config::KEYS[0])[0]);
			physical_server = ret.at(std::make_pair(port, host));

			if (physical_server.get() == NULL) {
				physical_server = ft::shared_ptr<PhysicalServer>(new PhysicalServer(host, port));
				ret.insert(std::make_pair(std::make_pair(port, host), physical_server));
				EventDto event_dto(physical_server->getSocket()->getFd(), physical_server);
				
				event_queue.pushEvent(factory.createEvent());
			}

			physical_server->addVirtualServer(server_configs[i]);
		} catch (const std::exception &e) {
			Logger::getInstance().error(e.what());
			throw (FailToConstructException());
		}
	}
	return (ret);
}

const Webserv::PhysicalServerMap	&Webserv::getPhysicalServerMap(void) const { return (this->_physical_server_map); }

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

std::string	Webserv::_initHost(const std::string &listen)  {
	std::string				ret = "0.0.0.0";
	std::string::size_type	pos = listen.find(':');

	if (pos != std::string::npos) {
		ret = listen.substr(0, pos);
	}
	return (ret);
}
int			Webserv::_initPort(const std::string &listen) {
	int						ret = 0;
	std::string::size_type	pos = listen.find(':');

	if (pos != std::string::npos) {
		ret = std::atoi(listen.substr(pos + 1).c_str());
	} else {
		ret = std::atoi(listen.c_str());
	}
	return (ret);
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