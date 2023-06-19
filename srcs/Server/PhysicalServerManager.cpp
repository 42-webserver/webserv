#include "../../incs/Server/PhysicalServerManager.hpp"
#include "../../incs/Server/Webserv.hpp"
#include <regex>

PhysicalServerManager::PhysicalServerManager() {}
PhysicalServerManager::~PhysicalServerManager() {}

bool	PhysicalServerManager::run(void) throw(std::exception) {
	for (PortMap::const_iterator it=this->_portMap.begin(); it!=this->_portMap.end(); ++it) {
		for (IpMap::const_iterator ipIt=it->second->begin(); ipIt!=it->second->end(); ++ipIt) {
			try {
				ipIt->second->run();
			} catch (const std::exception &e) {
				Logger::getInstance().error(e.what());
				throw (FailToRunException());
			}
		}
	}
	return (true);
}
bool	PhysicalServerManager::build(const Config &config) throw(std::exception) {
	try {
		this->_initPhysicalServers(config);
		this->_mergeWildCardIpMaps();
		this->_buildAllPhysicalServers();
		this->_registerAllListeningEvents();
	} catch (const std::exception &e) {
		Logger::getInstance().error(e.what());
		throw (FailToBuildException());
	}
}

bool	PhysicalServerManager::_initPhysicalServers(const Config &config) throw(std::exception) {
	const std::vector<Config::map> server_configs = config.getConfigMaps();

	if (MAX_SERVERS <= server_configs.size()) {
		throw (TooManyServerException());
	}

	for (size_t i=0; i<server_configs.size(); ++i) {
		try {
			int								port = _initPort(server_configs[i].at(Config::KEYS[Config::LISTEN])[0]);
			std::string						ip = _initIp(server_configs[i].at(Config::KEYS[Config::LISTEN])[0]);
			ft::shared_ptr<PhysicalServer>	physicalServer = this->findPhysicalServer(port, ip);

			if (physicalServer.get() == NULL) {
				physicalServer = ft::shared_ptr<PhysicalServer>(new PhysicalServer());
				this->_insertPhysicalServer(port, ip, physicalServer);
			}
			physicalServer->addVirtualServers(server_configs[i]);
		} catch (const std::exception &e) {
			Logger::getInstance().error(e.what());
			throw (FailToInitPhysicalServersException());
		}
	}

	return (true);
}
bool	PhysicalServerManager::_mergeWildCardIpMaps(void) {
	for (PortMap::const_iterator portIt=this->_portMap.begin(); portIt!=this->_portMap.end(); ++portIt) {
		if (this->_wildCardIpExists(portIt)) {
			this->_mergeIpMapsByPort(portIt);
		}
	}

	return (true);
}
bool	PhysicalServerManager::_buildAllPhysicalServers(void) throw(std::exception) {
	for (PortMap::const_iterator portIt=this->_portMap.begin(); portIt!=this->_portMap.end(); ++portIt) {
		for (IpMap::const_iterator ipIt=portIt->second->begin(); ipIt!=portIt->second->end(); ++ipIt) {
			try {
				ipIt->second->build(portIt->first, ipIt->first);
			} catch (const std::exception &e) {
				Logger::getInstance().error(e.what());
				throw (FailToBuildAllPhysicalServersException());
			}
		}
	}

	return (true);
}
bool	PhysicalServerManager::_registerAllListeningEvents(void) throw(std::exception) {
	EventQueue		&event_queue = EventQueue::getInstance();
	EventFactory	&eventFactory = ListenEventFactory::getInstance();

	for (PortMap::const_iterator portIt=this->_portMap.begin(); portIt!=this->_portMap.end(); ++portIt) {
		for (IpMap::const_iterator ipIt=portIt->second->begin(); ipIt!=portIt->second->end(); ++ipIt) {
			try {
				//event_queue.pushEvent(eventFactory.createEvent(ipIt->second));
			} catch (const std::exception &e) {
				Logger::getInstance().error(e.what());
				throw (FailToRegisterAllListeningEventsException());
			}
		}
	}
}

int														PhysicalServerManager::_initPort(const std::string &listen) throw(std::exception) {											
	std::string::size_type	pos = listen.find(':');
	int						ret = (pos != std::string::npos) ? std::atoi(listen.substr(pos + 1).c_str()) : std::atoi(listen.c_str());

	if (PhysicalServerManager::_portIsValid(ret) == false) {
		throw (InvalidPortException());
	}
	return (ret);
}
std::string												PhysicalServerManager::_initIp(const std::string &listen) throw(std::exception) {
	std::string::size_type	pos = listen.find(':');
	std::string				ret = (pos != std::string::npos) ? listen.substr(0, pos) : "0.0.0.0";

	if (PhysicalServerManager::_ipIsValid(ret) == false) {
		throw (InvalidIpException());
	}
	return (ret);
}
ft::shared_ptr<PhysicalServerManager::PhysicalServer>	PhysicalServerManager::findPhysicalServer(const int port, const std::string &ip) const {
	PortMap::const_iterator	portIt;
	IpMap::const_iterator	ipIt;

	if ((portIt = this->_portMap.find(port)) == this->_portMap.end() || \
		(ipIt = portIt->second->find(ip)) == portIt->second->end()) {
		return (ft::shared_ptr<PhysicalServer>(NULL));
	}

	return (ipIt->second);
}
bool													PhysicalServerManager::_insertPhysicalServer(const Port &port, const Ip &ip, const ft::shared_ptr<PhysicalServer> &physicalServer) {
	ft::shared_ptr<IpMap>	ipMap = ft::shared_ptr<IpMap>(new IpMap());

	ipMap->operator[](ip) = physicalServer;
	this->_portMap[port] = ipMap;
	return (true);
}
bool													PhysicalServerManager::_mergeIpMapsByPort(const PortMap::const_iterator &portIt) {
	ft::shared_ptr<IpMap>	ipMap = ft::shared_ptr<IpMap>(new IpMap());

	ipMap->operator[]("*") = ft::shared_ptr<PhysicalServer>(new PhysicalServer());
	for (IpMap::const_iterator ipMapIt=portIt->second->begin(); ipMapIt!=portIt->second->end(); ++ipMapIt) {
		ipMap->operator[]("*")->mergeVirtualServers(ipMapIt->second);
	}
	this->_portMap[portIt->first] = ipMap;

	return (true);
}

bool	PhysicalServerManager::_portIsValid(const Port &port) { return (0 < port && port < 65536); }
bool	PhysicalServerManager::_ipIsValid(const Ip &ip) {
	std::regex	ipv4Regex("^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$");

    return (std::regex_match(ip, ipv4Regex));
}
bool	PhysicalServerManager::_wildCardIpExists(const PortMap::const_iterator &portIt) {
	for (IpMap::const_iterator ipIt=portIt->second->begin(); ipIt!=portIt->second->end(); ++ipIt) {
		if (PhysicalServerManager::_ipIsWildCard(ipIt->first)) {
			return (true);
		}
	}

	return (false);
}
bool	PhysicalServerManager::_ipIsWildCard(const Ip &ip) { return (ip == "0.0.0.0" || ip == "*"); }

const char	*PhysicalServerManager::FailToBuildException::what() const throw() { return "PhysicalServerManager: Fail to build"; }
const char	*PhysicalServerManager::FailToRunException::what() const throw() { return "PhysicalServerManager: Fail to run"; }
const char	*PhysicalServerManager::TooManyServerException::what() const throw() { return "PhysicalServerManager: Too many servers"; }
const char	*PhysicalServerManager::InvalidPortException::what() const throw() { return "PhysicalServerManager: Invalid port"; }
const char	*PhysicalServerManager::InvalidIpException::what() const throw() { return "PhysicalServerManager: Invalid ip"; }
const char	*PhysicalServerManager::FailToInitPhysicalServersException::what() const throw() { return "PhysicalServerManager: Fail to init physical servers"; }
const char	*PhysicalServerManager::FailToBuildAllPhysicalServersException::what() const throw() { return "PhysicalServerManager: Fail to build all physical servers"; }
const char	*PhysicalServerManager::FailToRegisterAllListeningEventsException::what() const throw() { return "PhysicalServerManager: Fail to register all listening events"; }