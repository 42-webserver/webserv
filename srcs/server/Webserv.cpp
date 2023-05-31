#include "../../incs/server/Webserv.hpp"
#include <unistd.h>
#include "../../libs/unique_ptr/unique_ptr.hpp"

Webserv::Webserv(void): _servers(std::vector<Server>()) {}
Webserv::Webserv(const Config &config): _servers(std::vector<Server>()) {
	std::vector< Config::map >	server_configs = config.getConfigMaps();

	if (MAX_SERVERS <= server_configs.size()) {
		throw (TooManyServersException());
	}

	for (size_t i=0; i<server_configs.size(); ++i) {
		try {
			this->_servers.push_back(Server(server_configs[i]));
			
			int ListenFd = this->_servers.back().getSocket()->getFd();
			ListenEvFactory &factory = ListenEvFactory::getInstance();
			EventQueue &EvQueue = EventQueue::getInstance();

			EvQueue.pushEvent(factory.createEvent(ListenFd));
			std::cout << this->_servers.back() << '\n';
		} catch (const std::exception &e) {
			Logger::getInstance().error(e.what());
			throw (FailToConstructException());
		}
	}
}
Webserv::Webserv(const Webserv &ref): _servers(ref._servers) {}
Webserv::~Webserv(void) {}
Webserv	&Webserv::operator=(const Webserv &rhs) {
	if (this != &rhs) {
		this->~Webserv();
		new (this) Webserv(rhs);
	}
	return (*this);
}

bool	Webserv::run(void) throw(std::exception) {
	// Run server
	while (true) {
		EventQueue &EvQueue = EventQueue::getInstance();
		int	event_length;
		// Polling event
		try {
			event_length = EvQueue.pullEvents();
		} catch (const std::exception &e) {
			Logger::getInstance().error(e.what());
			throw (FailToRunException());
		}
		// Handle event
		for (int i=0; i<event_length; i++) {
			Event *event_data = EvQueue.getEventData(i);
			event_data->callEventHandler();
		}
	}
	return (true);
}

const char	*Webserv::TooManyServersException::what() const throw() { return ("Webserv: Too many server_configs"); }
const char	*Webserv::FailToConstructException::what() const throw() { return ("Webserv: Fail to construct"); }
const char	*Webserv::FailToRunException::what() const throw() { return ("Webserv: Fail to run"); }