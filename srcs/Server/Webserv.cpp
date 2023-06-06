#include "../../incs/Server/Webserv.hpp"
#include <unistd.h>
#include "../../libs/unique_ptr/unique_ptr.hpp"

Webserv::Webserv(void): _physical_server_map(PhysicalServerMap()) {}
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
	PhysicalServerMap				ret;

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
				const EventFactory				&factory = ListenEventFactory::getInstance();
				EventQueue 						&event_queue = EventQueue::getInstance();

				physical_server = ft::shared_ptr<PhysicalServer>(new PhysicalServer(host, port));
				ret.insert(std::make_pair(std::make_pair(port, host), physical_server));

				EventDto event_dto(physical_server->getSocket()->getFd(), physical_server->getVirtualServerMap());

				event_queue.pushEvent(factory.createEvent(event_dto));
			}

			physical_server->addVirtualServer(server_configs[i]);
		} catch (const std::exception &e) {
			Logger::getInstance().error(e.what());
			throw (FailToConstructException());
		}
	}
	return (ret);
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

std::string	Webserv::_initHost(const std::string &host)  {
	std::string	ret;
	std::string::size_type	prev_pos = 0;
	std::string::size_type	cur_pos = 0;
	size_t					number_of_dots = 0;

	while ((cur_pos = host.find('.', prev_pos)) != std::string::npos) {
		if (std::atoi(host.substr(prev_pos, cur_pos - prev_pos).c_str()) < 256) {
			ret += host.substr(prev_pos, cur_pos - prev_pos);
			ret += '.';
		}
		//  else {
		// 	throw (PhysicalServer::InvalidHostException());
		// }
		number_of_dots++;
		prev_pos = cur_pos + 1;
	}
	if (std::atoi(host.substr(prev_pos).c_str()) < 256) {
		ret += host.substr(prev_pos);
	} 
	// else {
	// 	throw (PhysicalServer::InvalidHostException());
	// }
	// if (number_of_dots != 3) {
	// 	throw (PhysicalServer::InvalidHostException());
	// }
	return (ret);
}

int			Webserv::_initPort(const std::string &host) {
	int	ret = std::atoi(host.c_str());

	// if (ret < 0 || 65535 < ret) {
	// 	throw (PhysicalServer::InvalidPortException());
	// }
	return (ret);
}

const char	*Webserv::TooManyServersException::what() const throw() { return ("Webserv: Too many server_configs"); }
const char	*Webserv::FailToConstructException::what() const throw() { return ("Webserv: Fail to construct"); }
const char	*Webserv::FailToRunException::what() const throw() { return ("Webserv: Fail to run"); }