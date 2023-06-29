#include "../../incs/Server/PhysicalServerManager.hpp"
#include "../../incs/EventQueue/EventQueue.hpp"
#include "../../incs/Event/ListenEvent.hpp"
#include "../../incs/Log/Logger.hpp"

PhysicalServerManager::PhysicalServerManager(void): _port_map(PortMap()) {}
PhysicalServerManager::~PhysicalServerManager(void) {}
PhysicalServerManager::PhysicalServerManager(const PhysicalServerManager &rhs): _port_map(rhs._port_map) {}
PhysicalServerManager	&PhysicalServerManager::operator=(const PhysicalServerManager &rhs) {
	if (this != &rhs) {
		this->~PhysicalServerManager();
		new (this) PhysicalServerManager(rhs);
	}
	return (*this);
}

bool	PhysicalServerManager::build(const Config &config) throw(std::exception) {
	try {
		const std::vector<Config::map>	config_maps = config.getConfigMaps();

		for (size_t i=0; i<config_maps.size(); i++) {
			this->_buildPhysicalServerVector(config_maps[i]);
		}
		for (PortMap::iterator it=this->_port_map.begin(); it!=this->_port_map.end(); it++) {
			if (this->_hasServerWithWildCardIp(it) == true) {
				this->_mergeAllPhysicalServer(it);
			}
		}
		this->_buildSocket();
	} catch (const std::exception &e) {
		Logger::getInstance().error(e.what());
		throw (FailToBuildException());
	}
	return (true);
}
bool	PhysicalServerManager::run(void) throw(std::exception) {
	try {
		for (PortMap::iterator port_it=this->_port_map.begin(); port_it!=this->_port_map.end(); port_it++) {
			for (PhysicalServerVector::iterator ps_it=port_it->second.begin(); ps_it!=port_it->second.end(); ps_it++) {
				// runSocket
				(*ps_it)->run();
				// initEventQueue
				// initListenEvent
			}
		}
	} catch (const std::exception &e) {
		Logger::getInstance().error(e.what());
		throw (FailToBuildException());
	}
	return (true);
}

ft::shared_ptr<PhysicalServer>	PhysicalServerManager::findPhysicalServer(const Port &port, const Ip &ip) const {
	PortMap::const_iterator			it;
	PhysicalServerVector			physical_server_vector;
	ft::shared_ptr<VirtualServer>	virtual_server;

	if ((it = this->_port_map.find(port)) == this->_port_map.end()) {
		return (ft::shared_ptr<PhysicalServer>(NULL));
	}
	physical_server_vector = it->second;
	for (size_t i=0; i<physical_server_vector.size(); i++) {
		virtual_server = physical_server_vector[i]->findVirtualServerByIp(ip);
		if (virtual_server.get() != NULL) {
			return (physical_server_vector[i]);
		}
	}
	return (ft::shared_ptr<PhysicalServer>(NULL));
}
ft::shared_ptr<PhysicalServer>	PhysicalServerManager::_findPhysicalServerByIp(const PortMap::const_iterator &it, const Ip &ip) const {
	PhysicalServerVector			physical_server_vector = it->second;
	ft::shared_ptr<VirtualServer>	virtual_server;

	for (size_t i=0; i<physical_server_vector.size(); i++) {
		virtual_server = physical_server_vector[i]->findVirtualServerByIp(ip);
		if (virtual_server.get() != NULL) {
			return (physical_server_vector[i]);
		}
	}
	return (ft::shared_ptr<PhysicalServer>(NULL));
}

bool	PhysicalServerManager::_buildPhysicalServerVector(const Config::map &config_map) throw(std::exception) {
	const std::string				listen = config_map.find("listen")->second[0];
	const Port						port = _parsePort(listen);
	const Ip						ip = _parseIp(listen);
	PortMap::iterator				it;
	ft::shared_ptr<PhysicalServer>	physical_server;

	if ((it = this->_port_map.find(port)) == this->_port_map.end()) {
		this->_port_map.insert(std::make_pair(port, PhysicalServerVector()));
		it = this->_port_map.find(port);
	}
	if ((physical_server = this->_findPhysicalServerByIp(it, ip)).get() == NULL) {
		physical_server = ft::shared_ptr<PhysicalServer>(new PhysicalServer());
		it->second.push_back(physical_server);
	}
	try {
		physical_server->build(ip, config_map);
	} catch (const std::exception &e) {
		Logger::getInstance().error(e.what());
		throw (FailToBuildPhysicalServerVectorException());
	}
	return (true);
}
bool	PhysicalServerManager::_buildSocket(void) throw(std::exception) {
	try {
		for (PortMap::iterator map_it=this->_port_map.begin(); map_it!=this->_port_map.end(); map_it++) {
			for (PhysicalServerVector::iterator ps_it=map_it->second.begin(); ps_it!=map_it->second.end(); ps_it++) {
				(*ps_it)->buildSocket(map_it->first);
			}
		}
	} catch (const std::exception &e) {
		Logger::getInstance().error(e.what());
		throw (FailToBuildSocketException());
	}
	return (true);
}
bool	PhysicalServerManager::_hasServerWithWildCardIp(const PortMap::const_iterator &it) const {
	PhysicalServerVector	physical_server_vector = it->second;

	for (size_t i=0; i<physical_server_vector.size(); i++) {
		if (physical_server_vector[i]->hasServerWithWildCardIp() == true) {
			return (true);
		}
	}
	return (false);
}
bool	PhysicalServerManager::_mergeAllPhysicalServer(const PortMap::iterator &it) throw(std::exception) {
	PhysicalServerVector			physical_server_vector = it->second;
	ft::shared_ptr<PhysicalServer>	physical_server = physical_server_vector[0];

	try {
		for (size_t i=1; i<physical_server_vector.size(); i++) {
			physical_server->mergeAllVirtualServer(physical_server_vector[i]);
		}
		it->second = PhysicalServerVector(1, physical_server);
	} catch (const std::exception &e) {
		Logger::getInstance().error(e.what());
		throw (FailToMergeAllPhysicalServerException());
	}
	return (true);
}

PhysicalServerManager::Port	PhysicalServerManager::_parsePort(const std::string &listen) throw(std::exception) {											
	std::string::size_type	pos = listen.find(':');
	int						ret = (pos != std::string::npos) ? std::atoi(listen.substr(pos + 1).c_str()) : std::atoi(listen.c_str());

	if (PhysicalServerManager::_portIsValid(ret) == false) {
		throw (InvalidPortException());
	}
	return (ret);
}
PhysicalServerManager::Ip	PhysicalServerManager::_parseIp(const std::string &listen) throw(std::exception) {
	std::string::size_type	pos = listen.find(':');
	std::string				ret = (pos != std::string::npos) ? listen.substr(0, pos) : "";

	ret = (ret == "" || ret == "*") ? "0.0.0.0" : ret;
	if (PhysicalServerManager::_ipIsValid(ret) == false) {
		throw (InvalidIpException());
	}
	return (ret);
}
bool						PhysicalServerManager::_portIsValid(const Port &port) { return (0 < port && port < 65536); }
bool						PhysicalServerManager::_ipIsValid(const Ip &ip) {
	std::string::size_type	prev_pos = 0, cur_pos;
	size_t					num;

	for (size_t i=0; i<3; i++) {
		cur_pos = ip.find('.', prev_pos);
		num = std::atoi(ip.substr(prev_pos, cur_pos - prev_pos).c_str());
		if (num < 0 || 255 < num) {
			return (false);
		}
		prev_pos = cur_pos + 1;
	}

	num = std::atoi(ip.substr(prev_pos).c_str());
	if (num < 0 || 255 < num) {
		return (false);
	}

	return (true);
}

const char	*PhysicalServerManager::FailToBuildException::what() const throw() { return "PhysicalServerManager: Fail to build"; }
const char	*PhysicalServerManager::FailToRunException::what() const throw() { return "PhysicalServerManager: Fail to run"; }
const char	*PhysicalServerManager::InvalidPortException::what() const throw() { return ("PhysicalServerManager: Invalid port"); }
const char	*PhysicalServerManager::InvalidIpException::what() const throw() { return "PhysicalServerManager: Invalid ip"; }
const char	*PhysicalServerManager::TooManyServerException::what() const throw() { return ("PhysicalServerManager: Too many servers"); }
const char	*PhysicalServerManager::FailToMergeAllPhysicalServerException::what() const throw() { return ("PhysicalServerManager: Fail to merge all physical server"); }
const char	*PhysicalServerManager::FailToBuildPhysicalServerVectorException::what() const throw() { return ("PhysicalServerManager: Fail to build physical server vector"); }
const char	*PhysicalServerManager::FailToBuildSocketException::what() const throw() { return ("PhysicalServerManager: Fail to build socket"); }

std::ostream	&operator<<(std::ostream &os, const PhysicalServerManager &physical_server_manager) {
	os << "\t" << "PhysicalServerManager:\n";
	for (PhysicalServerManager::PortMap::const_iterator it=physical_server_manager._port_map.begin(); it!=physical_server_manager._port_map.end(); it++) {
		os << "\t\t" << "Port: " << it->first << '\n';
		for (size_t i=0; i<it->second.size(); i++) {
			os << *(it->second[i]);
			if (i + 1 < it->second.size()) {
				os << '\n';
			}
		}
	}
	return (os);
}