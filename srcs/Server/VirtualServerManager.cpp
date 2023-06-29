#include "../../incs/Server/VirtualServerManager.hpp"
#include "../../incs/Log/Logger.hpp"
#include <fstream>
#include <sstream>

//VirtualServerManager::hostsMap VirtualServerManager::hostsFromFile = VirtualServerManager::hostsMap();

VirtualServerManager::VirtualServerManager(void): _ip_map(IpMap()), _server_name_map(ServerNameMap()) {
	//parseHostsFile();
}
VirtualServerManager::~VirtualServerManager(void) {}
VirtualServerManager::VirtualServerManager(const VirtualServerManager &ref): _ip_map(ref._ip_map), _server_name_map(ref._server_name_map) {}
VirtualServerManager	&VirtualServerManager::operator=(const VirtualServerManager &rhs) {
	if (this != &rhs) {
		this->~VirtualServerManager();
		new(this) VirtualServerManager(rhs);
	}
	return (*this);
}

bool	VirtualServerManager::build(const Ip &ip, const Config::map &config_map) throw(std::exception) {
	try {
		ft::shared_ptr<VirtualServer>	virtual_server(new VirtualServer(config_map));
		std::vector<ServerName>			server_names = config_map.at(Config::KEYS[Config::KEY::SERVER_NAME]);

		if (this->_ip_map.find(ip) == this->_ip_map.end()) {
			this->_ip_map.insert(std::make_pair(ip, virtual_server));
		}
		for (size_t i=0; i<server_names.size(); i++) {
			if (this->_server_name_map.find(server_names[i]) != this->_server_name_map.end()) {
				throw (DuplicatedServerNameException());
			}
			this->_server_name_map.insert(std::make_pair(server_names[i], virtual_server));
		}
	} catch (std::exception &e) {
		Logger::getInstance().error(e.what());
		throw (FailToBuildException());
	}

	return (true);
}
bool	VirtualServerManager::hasServerWithWildCardIp(void) const {
	for (IpMap::const_iterator it=this->_ip_map.begin(); it!=this->_ip_map.end(); ++it) {
		if (it->first == "0.0.0.0") {
			return (true);
		}
	}
	return (false);
}
bool	VirtualServerManager::mergeAllVirtualServer(const ft::shared_ptr<VirtualServerManager> &other) throw(std::exception) {
	for (IpMap::const_iterator it=other->_ip_map.begin(); it!=other->_ip_map.end(); ++it) {
		if (this->_ip_map.find(it->first) != this->_ip_map.end()) {
			continue;
		}
		this->_ip_map.insert(std::make_pair(it->first, it->second));
	}
	for (ServerNameMap::const_iterator it=other->_server_name_map.begin(); it!=other->_server_name_map.end(); ++it) {
		if (this->_server_name_map.find(it->first) != this->_server_name_map.end()) {
			throw (DuplicatedServerNameException());
		}
		this->_server_name_map.insert(std::make_pair(it->first, it->second));
	}
	return (true);
}

ft::shared_ptr<VirtualServer>	VirtualServerManager::findVirtualServerByIp(const Ip &ip) const {
	IpMap::const_iterator	it = this->_ip_map.find(ip);

	if (it == this->_ip_map.end()) {
		return (ft::shared_ptr<VirtualServer>(NULL));
	}
	return (it->second);
}
ft::shared_ptr<VirtualServer>	VirtualServerManager::findVirtualServerByName(const ServerName &server_name) const {
	ServerNameMap::const_iterator	it = this->_server_name_map.find(server_name);

	if (it== this->_server_name_map.end()) {
		return (ft::shared_ptr<VirtualServer>(NULL));
	}
	return (it->second);
}

//bool	registerListeningEvent() {
//	EventQueue		&eventQueue = EventQueue::getInstance();
//	EventFactory	&eventFactory = ListeningEventFactory::getInstance();

//	eventQueue.pushEvent(eventFactory.createEvent(EventDto()));
//}

//void VirtualServerManager::parseHostsFile() {
//	if (!hostsFromFile.empty()) {
//		return;
//	}

//	std::ifstream hostsFile("/etc/hosts");
//	if (!hostsFile.is_open()) {
//		return;
//	}

//	std::string line;
//	while (std::getline(hostsFile, line)) {
//		if (line[0] == '#') {
//			continue;
//		}

//		std::istringstream iss(line);
//		std::string ip, host;
		
//		if (!(iss >> ip >> host)) {
//			break;
//		}
//		hostsFromFile[host] = ip;
//	}

//	hostsFile.close();
//}

//bool VirtualServerManager::isInEtcHosts(const std::string& host) {
//	return hostsFromFile.count(host) > 0;
//}

const char	*VirtualServerManager::FailToBuildException::what() const throw() { return ("VirtualServerManager: Fail to build"); }
const char	*VirtualServerManager::DuplicatedServerNameException::what() const throw() { return ("VirtualServerManager: Duplicated server name"); }
const char	*VirtualServerManager::FailToMergeAllVirtualServerException::what() const throw() { return ("VirtualServerManager: Fail to merge all virtual server"); }

std::ostream	&operator<<(std::ostream &os, const VirtualServerManager &virtual_server_manager) {
	os << "\t\t\t\t" << "VirtualServerManager:" << '\n';
	os << "\t\t\t\t\t" << "IpMap:" << '\n';
	for (VirtualServerManager::IpMap::const_iterator it = virtual_server_manager._ip_map.begin(); it != virtual_server_manager._ip_map.end(); ++it) {
		os << "\t\t\t\t\t\t" << it->first << ":" << '\n';
		os << *(it->second);
	}
	os << "\t\t\t\t\t" << "ServerNameMap:" << '\n';
	for (VirtualServerManager::ServerNameMap::const_iterator it = virtual_server_manager._server_name_map.begin(); it != virtual_server_manager._server_name_map.end(); ++it) {
		os << "\t\t\t\t\t\t" << it->first << ":" << '\n';
		os << *(it->second);
	}
	return (os);
}