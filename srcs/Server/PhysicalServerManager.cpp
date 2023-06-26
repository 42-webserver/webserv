#include "../../incs/Server/PhysicalServerManager.hpp"
#include "../../incs/EventQueue/EventQueue.hpp"
#include "../../incs/Event/ListenEvent.hpp"
#include "../../incs/Log/Logger.hpp"
PhysicalServerManager::PhysicalServerManager(void): _port_map(PortMap()) {}
PhysicalServerManager::~PhysicalServerManager(void) {}

bool	PhysicalServerManager::build(const Config &config) throw(std::exception) {
	try {
		this->_initPhysicalServers(config);
		this->_mergeWildCardIpTries();
		this->_buildAllPhysicalServers();
		this->_registerAllListeningEvents();
	} catch (const std::exception &e) {
		Logger::getInstance().error(e.what());
		throw (FailToBuildException());
	}

	return (true);
}
bool	PhysicalServerManager::run(void) throw(std::exception) {
	for (PortMap::const_iterator it=this->_port_map.begin(); it!=this->_port_map.end(); ++it) {
		for (IpTrie::const_iterator ipIt=it->second->begin(); ipIt!=it->second->end(); ++ipIt) {
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

ft::shared_ptr<PhysicalServerManager::PhysicalServer>	PhysicalServerManager::findPhysicalServer(const int port, const std::string &ip) const {
	PortMap::const_iterator	portIt;
	IpTrie::const_iterator	ipIt;

	if ((portIt = this->_port_map.find(port)) == this->_port_map.end() || \
		(ipIt = portIt->second->find(ip)) == portIt->second->end()) {
		return (ft::shared_ptr<PhysicalServer>(NULL));
	}

	return (ipIt->second);
}

bool	PhysicalServerManager::_initPhysicalServers(const Config &config) throw(std::exception) {
	const std::vector<Config::map> server_configs = config.getConfigMaps();

	if (MAX_SERVERS <= server_configs.size()) {
		throw (TooManyServerException());
	}

	for (size_t i=0; i<server_configs.size(); ++i) {
		try {
			int								port = _parsePort(server_configs[i].at(Config::KEYS[Config::KEY::LISTEN])[0]);
			std::string						ip = _parseIp(server_configs[i].at(Config::KEYS[Config::KEY::LISTEN])[0]);
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
bool	PhysicalServerManager::_mergeWildCardIpTries(void) {
	for (PortMap::const_iterator portIt=this->_port_map.begin(); portIt!=this->_port_map.end(); ++portIt) {
		if (this->_wildCardIpExists(portIt)) {
			this->_mergeIpTriesByPort(portIt);
		}
	}

	return (true);
}
bool	PhysicalServerManager::_buildAllPhysicalServers(void) throw(std::exception) {
	for (PortMap::const_iterator portIt=this->_port_map.begin(); portIt!=this->_port_map.end(); ++portIt) {
		for (IpTrie::const_iterator ipIt=portIt->second->begin(); ipIt!=portIt->second->end(); ++ipIt) {
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
	//for (PortMap::const_iterator portIt=this->_port_map.begin(); portIt!=this->_port_map.end(); ++portIt) {
	//	for (IpTrie::const_iterator ipIt=portIt->second->begin(); ipIt!=portIt->second->end(); ++ipIt) {
	//		try {
	//			ft::shared_ptr<VirtualServerManager>	vsm = ipIt->second;
	//			vsm->registerListeningEvents();
	//		} catch (const std::exception &e) {
	//			Logger::getInstance().error(e.what());
	//			throw (FailToRegisterAllListeningEventsException());
	//		}
	//	}
	//}
	
	return (true);
}
bool	PhysicalServerManager::_insertPhysicalServer(const Port &port, const Ip &ip, const ft::shared_ptr<PhysicalServer> &physicalServer) {
	ft::shared_ptr<IpTrie>	IpTrie = ft::shared_ptr<IpTrie>(new IpTrie());

	IpTrie->operator[](ip) = physicalServer;
	this->_port_map[port] = IpTrie;
	return (true);
}
bool	PhysicalServerManager::_mergeIpTriesByPort(const PortMap::const_iterator &portIt) {
	ft::shared_ptr<IpTrie>	IpTrie = ft::shared_ptr<IpTrie>(new IpTrie());

	IpTrie->operator[]("0.0.0.0") = ft::shared_ptr<PhysicalServer>(new PhysicalServer());
	for (IpTrie::const_iterator IpTrieIt=portIt->second->begin(); IpTrieIt!=portIt->second->end(); ++IpTrieIt) {
		IpTrie->operator[]("0.0.0.0")->mergeVirtualServers(IpTrieIt->second);
	}
	this->_port_map[portIt->first] = IpTrie;

	return (true);
}

int			PhysicalServerManager::_parsePort(const std::string &listen) throw(std::exception) {											
	std::string::size_type	pos = listen.find(':');
	int						ret = (pos != std::string::npos) ? std::atoi(listen.substr(pos + 1).c_str()) : std::atoi(listen.c_str());

	if (PhysicalServerManager::_portIsValid(ret) == false) {
		throw (InvalidPortException());
	}
	return (ret);
}
std::string	PhysicalServerManager::_parseIp(const std::string &listen) throw(std::exception) {
	std::string::size_type	pos = listen.find(':');
	std::string				ret = (pos != std::string::npos) ? listen.substr(0, pos) : "";

	ret = (ret == "" || ret == "*") ? "0.0.0.0" : ret;
	if (PhysicalServerManager::_ipIsValid(ret) == false) {
		throw (InvalidIpException());
	}
	return (ret);
}
bool		PhysicalServerManager::_portIsValid(const Port &port) { return (0 < port && port < 65536); }
bool		PhysicalServerManager::_ipIsValid(const Ip &ip) {
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
bool		PhysicalServerManager::_wildCardIpExists(const PortMap::const_iterator &portIt) {
	for (IpTrie::const_iterator ipIt=portIt->second->begin(); ipIt!=portIt->second->end(); ++ipIt) {
		if (PhysicalServerManager::_ipIsWildCard(ipIt->first)) {
			return (true);
		}
	}

	return (false);
}
bool		PhysicalServerManager::_ipIsWildCard(const Ip &ip) { return (ip == "0.0.0.0"); }

const char	*PhysicalServerManager::FailToBuildException::what() const throw() { return "PhysicalServerManager: Fail to build"; }
const char	*PhysicalServerManager::FailToRunException::what() const throw() { return "PhysicalServerManager: Fail to run"; }
const char	*PhysicalServerManager::TooManyServerException::what() const throw() { return "PhysicalServerManager: Too many servers"; }
const char	*PhysicalServerManager::InvalidPortException::what() const throw() { return "PhysicalServerManager: Invalid port"; }
const char	*PhysicalServerManager::InvalidIpException::what() const throw() { return "PhysicalServerManager: Invalid ip"; }
const char	*PhysicalServerManager::FailToInitPhysicalServersException::what() const throw() { return "PhysicalServerManager: Fail to init physical servers"; }
const char	*PhysicalServerManager::FailToBuildAllPhysicalServersException::what() const throw() { return "PhysicalServerManager: Fail to build all physical servers"; }
const char	*PhysicalServerManager::FailToRegisterAllListeningEventsException::what() const throw() { return "PhysicalServerManager: Fail to register all listening events"; }