#include "../../incs/Server/PhysicalServerManager.hpp"
#include "../../incs/Server/Webserv.hpp"

PhysicalServerManager::PhysicalServerManager() {}

void PhysicalServerManager::insertVirtualServer(port port, ip ip, const Config::map &config_map, const std::vector<std::string> &server_names) {
    // Create a new VirtualServer instance
    ft::shared_ptr<VirtualServer> new_virtual_server(new VirtualServer(config_map));

    // Handle the port and IP
    std::map<PhysicalServerManager::port, std::multimap<
	PhysicalServerManager::ip, ft::shared_ptr<VirtualServer> > >::iterator port_it = _portMap.find(port);
    if (port_it == _portMap.end()) {
        // If the port does not exist, create a new IP map and insert it into the port map
        // why MultiMap? because we can have multiple virtual servers with the same IP
        std::multimap<PhysicalServerManager::ip, ft::shared_ptr<VirtualServer> > new_ip_map;
        new_ip_map.insert(std::make_pair(ip, new_virtual_server));
        _portMap.insert(std::make_pair(port, new_ip_map));
    } else {
        // If the port exists, just insert the new VirtualServer into the corresponding IP map
        std::multimap<PhysicalServerManager::ip, ft::shared_ptr<VirtualServer> >& ip_map = port_it->second;
        ip_map.insert(std::make_pair(ip, new_virtual_server));
    }

    // Handle the server names
    for (std::vector<std::string>::const_iterator it = server_names.begin(); it != server_names.end(); ++it) {
        if (_domainMap.find(*it) != _domainMap.end()) {
            throw DuplicateServerException();
        }
        _domainMap.insert(std::make_pair(*it, new_virtual_server));
    }
}

bool PhysicalServerManager::checkWildcardIP(const std::multimap<ip, 
ft::shared_ptr<VirtualServer> >& ipMultimap) {
    // Check if wildcard IP exists in current port using find
    return ipMultimap.find("0.0.0.0") != ipMultimap.end();
}

std::map<PhysicalServerManager::ip, ft::shared_ptr<VirtualServer> > 
PhysicalServerManager::buildIPMap(const std::multimap<ip, ft::shared_ptr<VirtualServer> >& ipMultimap, 
bool isWildcard, ip targetIP) {
    std::map<ip, ft::shared_ptr<VirtualServer> > ip_map;
    if (isWildcard) {
        // If wildcard IP exists, copy the whole map
        for (std::multimap<ip, ft::shared_ptr<VirtualServer> >::const_iterator ip_it = ipMultimap.begin();
        ip_it != ipMultimap.end(); ++ip_it) {
            ip_map[ip_it->first] = ip_it->second;
        }
    } else {
        // If wildcard IP does not exist, copy only target IP
        ip_map[targetIP] = ipMultimap.find(targetIP)->second;
    }
    return ip_map;
}

std::map<PhysicalServerManager::serverName, ft::shared_ptr<VirtualServer> > 
PhysicalServerManager::buildDomainMap(const std::map<ip, ft::shared_ptr<VirtualServer> >& ipMap) {
    std::map<serverName, ft::shared_ptr<VirtualServer> > currentDomainMap;
    for (std::map<serverName, ft::shared_ptr<VirtualServer> >::const_iterator it = 
	_domainMap.begin(); it != _domainMap.end(); ++it) {
        // Virtual Server Has ip Because Compare Domain with Ip this May be not good bb
        if (ipMap.count(it->second->getIP()) > 0) {
            currentDomainMap[it->first] = it->second;
        }
    }
    return currentDomainMap;
}

void PhysicalServerManager::createAndStoreVirtualServerManager(std::map<ip, ft::shared_ptr<VirtualServer> >& ipMap, 
std::map<serverName, ft::shared_ptr<VirtualServer> >& domainMap) {
    VirtualServerManager vsm;
    vsm.buildFromPhysicalServerManager(ipMap, domainMap);
    _VirtualServerManagers.push_back(vsm);
}

void PhysicalServerManager::buildAllVirtualServerManagers() {
    for (std::map<port, std::multimap<ip, ft::shared_ptr<VirtualServer> > >::const_iterator port_it = _portMap.begin(); 
	port_it != _portMap.end(); ++port_it) {
        bool isWildcard = checkWildcardIP(port_it->second);

        if (isWildcard) {
            // If wildcard IP exists, create one VirtualServerManager
            std::map<ip, ft::shared_ptr<VirtualServer> > ip_map = buildIPMap(port_it->second, true, "");
            std::map<serverName, ft::shared_ptr<VirtualServer> > domainMap = buildDomainMap(ip_map);
            createAndStoreVirtualServerManager(ip_map, domainMap);
        } else {
            // If wildcard IP does not exist, create a VirtualServerManager for each IP
            for (std::multimap<ip, ft::shared_ptr<VirtualServer> >::const_iterator ip_it = port_it->second.begin();
            ip_it != port_it->second.end(); ++ip_it) {
                std::map<ip, ft::shared_ptr<VirtualServer> > ip_map = buildIPMap(port_it->second, false, ip_it->first);
                std::map<serverName, ft::shared_ptr<VirtualServer> > domainMap = buildDomainMap(ip_map);
                createAndStoreVirtualServerManager(ip_map, domainMap);
            }
        }
    }

    if (!_VirtualServerManagers.empty()) {
        _vsmit = _VirtualServerManagers.begin();
    }
}


void PhysicalServerManager::run(const Config &config) throw(std::exception) {
    insertAllVirtualServers(config);
    buildAllVirtualServerManagers();
}

VirtualServerManager PhysicalServerManager::getCurrentVirtualServerManager() {
    if (_vsmit == _VirtualServerManagers.end()) {
        _vsmit = _VirtualServerManagers.begin();
    }
    if (_vsmit == _VirtualServerManagers.end()) {
        throw std::runtime_error("No VirtualServerManager exists");
    }
    return *_vsmit++;
}

int PhysicalServerManager::getVirtualServerManagerCount() {
    return _VirtualServerManagers.size();
}
void PhysicalServerManager::insertAllVirtualServers(const Config &config) {
    const std::vector<Config::map> server_configs = config.getConfigMaps();

    if (MAX_SERVERS <= server_configs.size()) {
        throw ((FailToConstructException()));
    }

    for (std::vector<Config::map>::const_iterator curServerConfig = server_configs.begin(); 
    curServerConfig != server_configs.end(); ++curServerConfig) {
        std::string host;
        int port;

        try {
            host = _initHost(curServerConfig->at(Config::KEYS[ConfigKey::LISTEN])[0]);
            port = _initPort(curServerConfig->at(Config::KEYS[ConfigKey::LISTEN])[0]);
            insertVirtualServer(port, host, *curServerConfig, curServerConfig->at(Config::KEYS[ConfigKey::SERVER_NAME]));
        } catch (const std::exception &e) {
            Logger::getInstance().error(e.what());
            throw (FailToConstructException());
        }
    }

}

std::string	PhysicalServerManager::_initHost(const std::string &listen)  {
	std::string				ret = "0.0.0.0";
	std::string::size_type	pos = listen.find(':');

	if (pos != std::string::npos) {
		ret = listen.substr(0, pos);
	}
	return (ret);
}
int			PhysicalServerManager::_initPort(const std::string &listen) {
	int						ret = 0;
	std::string::size_type	pos = listen.find(':');

	if (pos != std::string::npos) {
		ret = std::atoi(listen.substr(pos + 1).c_str());
	} else {
		ret = std::atoi(listen.c_str());
	}
	return (ret);
}

const char *PhysicalServerManager::FailToConstructException::what() const throw() {
    return "Fail to construct PhysicalServerManager";
}

const char *PhysicalServerManager::DuplicateServerException::what() const throw() {
    return "Duplicate server name";
}