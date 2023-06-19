#include "../../incs/Server/PhysicalServerManager.hpp"
#include "../../incs/Server/Webserv.hpp"

PhysicalServerManager::PhysicalServerManager() {}
PhysicalServerManager::~PhysicalServerManager() {}

bool	PhysicalServerManager::run(const Config &config) throw(std::exception) {
	try {
		this->_init(config);
		this->_build();
	} catch (const std::exception &e) {
		Logger::getInstance().error(e.what());
		throw (FailToRun());
	}
	// run all physical servers (Virtual Server Managers)
}

bool	PhysicalServerManager::_init(const Config &config) {
	const std::vector<Config::map> server_configs = config.getConfigMaps();

	if (MAX_SERVERS <= server_configs.size()) {
		throw (TooManyServerException());
	}

	for (size_t i=0; i<server_configs.size(); ++i) {
		try {
			std::string						ip = _initIp(server_configs[i].at(Config::KEYS[ConfigKey::LISTEN])[0]);
			int								port = _initPort(server_configs[i].at(Config::KEYS[ConfigKey::LISTEN])[0]);
			ft::shared_ptr<PhysicalServer>	physicalServer = this->findPhysicalServer(port, ip);

			if (physicalServer.get() == NULL) {
				physicalServer = this->_initPhysicalServer();
				this->_insertPhysicalServer(port, ip, physicalServer);
			}
			this->_insertVirtualServerToPhysicalServer(physicalServer, server_configs[i]);
		} catch (const std::exception &e) {
			Logger::getInstance().error(e.what());
			throw (FailToInit());
		}
	}

	return (true);
}

bool	PhysicalServerManager::_build(void) throw(std::exception) {
	try {
		for (PortMap::const_iterator portIt=this->_portMap.begin(); portIt!=this->_portMap.end(); ++portIt) {
			if (this->_wildCardIpExists(portIt)) {
				this->_mergeIpMapByPort(portIt);
			}

			for (IpMap::const_iterator ipMapIt=portIt->second.begin(); ipMapIt!=portIt->second.end(); ++ipMapIt) {
				//ipMapIt->second->build();
			}
		}
	} catch (const std::exception &e) {
		Logger::getInstance().error(e.what());
		throw (FailToBuild());
	}
}

int			PhysicalServerManager::_initPort(const std::string &listen) {
	std::string::size_type	pos = listen.find(':');
	int						ret = (pos != std::string::npos) ? std::atoi(listen.substr(pos + 1).c_str()) : std::atoi(listen.c_str());

	return (ret);
}
std::string	PhysicalServerManager::_initIp(const std::string &listen)  {
	std::string::size_type	pos = listen.find(':');
	std::string				ret = (pos != std::string::npos) ? listen.substr(0, pos) : "0.0.0.0";

	return (ret);
}

ft::shared_ptr<PhysicalServerManager::PhysicalServer>	PhysicalServerManager::findPhysicalServer(const int port, const std::string &ip) const {
	PortMap::const_iterator	portIt;
	IpMap::const_iterator	ipIt;

	if ((portIt = this->_portMap.find(port)) == this->_portMap.end() || \
		(ipIt = portIt->second.find(ip)) == portIt->second.end()) {
		return (ft::shared_ptr<PhysicalServer>(NULL));
	}

	return (ipIt->second);
}
ft::shared_ptr<PhysicalServerManager::PhysicalServer>	PhysicalServerManager::_initPhysicalServer(void) const {
	ft::shared_ptr<PhysicalServer>	physicalServer(new PhysicalServer());

	return (physicalServer);
}
bool													PhysicalServerManager::_insertPhysicalServer(const Port port, const Ip &ip, ft::shared_ptr<PhysicalServer> physicalServer) {
	IpMap	ipMap;

	ipMap[ip] = physicalServer;
	this->_portMap[port] = ipMap;
	return (true);
}
bool													PhysicalServerManager::_mergeIpMapByPort(const PortMap::const_iterator portIt) throw(std::excetpion) {
	IpMap	ipMap;

	ipMap["*"] = ft::shared_ptr<PhysicalServer>(NULL);
	for (IpMap::const_iterator ipMapIt=portIt->second.begin(); ipMapIt!=portIt->second.end(); ++ipMapIt) {
		//ipMap["*"].addVirtualServer(ipMapIt->second);
	}
	this->_portMap[portIt->first] = ipMap;

	return (true);
}

bool	PhysicalServerManager::_wildCardIpExists(const PortMap::const_iterator portIt) const {
	for (IpMap::const_iterator ipIt=portIt->second.begin(); ipIt!=portIt->second.end(); ++ipIt) {
		if (this->_ipIsWildCard(ipIt->first)) {
			return (true);
		}
	}
	return (false);
}
bool	PhysicalServerManager::_ipIsWildCard(const Ip &ip) const { return (ip == "0.0.0.0" || ip == "*"); }

const char	*PhysicalServerManager::FailToRun::what() const throw() { return "PhysicalServerManager: Fail to run"; }
const char	*PhysicalServerManager::TooManyServerException::what() const throw() { return "PhysicalServerManager: Too many servers"; }
const char	*PhysicalServerManager::FailToInit::what() const throw() { return "PhysicalServerManager: Fail to init"; }
const char	*PhysicalServerManager::FailToBuild::what() const throw() { return "PhysicalServerManager: Fail to build"; }
//const char	*PhysicalServerManager::DuplicatedServerException::what() const throw() { return "PhysicalServerManager: Duplicated server name"; }