#include "../../incs/Server/VirtualServerManager.hpp"
#include "../../incs/Log/Logger.hpp"
#include <fstream>
#include <sstream>

//VirtualServerManager::hostsMap VirtualServerManager::hostsFromFile = VirtualServerManager::hostsMap();

VirtualServerManager::VirtualServerManager(void) {
	//parseHostsFile();
}
VirtualServerManager::~VirtualServerManager(void) {}

bool	VirtualServerManager::build(const Ip &ip, const Config::map &config_map) throw(std::exception) {
	try {
		ft::shared_ptr<VirtualServer>	virtual_server(new VirtualServer(config_map));
		std::vector<ServerName>			server_names = config_map.at(Config::KEYS[Config::KEY::SERVER_NAME]);

		this->_virtual_server_vector.push_back(std::make_pair(ip, virtual_server));
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
bool	VirtualServerManager::run(void) throw(std::exception) {
	try {
		// TODO
	} catch (std::exception &e) {
		Logger::getInstance().error(e.what());
		throw (FailToRunException());
	}

	return (true);
}
bool	VirtualServerManager::hasServerWithWildCardIp(void) const {
	for (size_t i=0; i<this->_virtual_server_vector.size(); ++i) {
		if (this->_virtual_server_vector[i].first == "0.0.0.0") {
			return (true);
		}
	}
	return (false);
}
bool	VirtualServerManager::mergeAllVirtualServer(const ft::shared_ptr<VirtualServerManager> &other) throw(std::exception) {
	for (size_t i=0; i<other->_virtual_server_vector.size(); ++i) {
		this->_virtual_server_vector.push_back(other->_virtual_server_vector[i]);
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
	for (size_t i=0; i<this->_virtual_server_vector.size(); ++i) {
		if (this->_virtual_server_vector[i].first == ip) {
			return (this->_virtual_server_vector[i].second);
		}
	}
	return (ft::shared_ptr<VirtualServer>(NULL));
}
ft::shared_ptr<VirtualServer>	VirtualServerManager::findVirtualServerByName(const ServerName &server_name) const {
	ServerNameMap::const_iterator	it;

	if ((it = this->_server_name_map.find(server_name)) == this->_server_name_map.end()) {
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
const char	*VirtualServerManager::FailToRunException::what() const throw() { return ("VirtualServerManager: Fail to run"); }
const char	*VirtualServerManager::DuplicatedServerNameException::what() const throw() { return ("VirtualServerManager: Duplicated server name"); }
const char	*VirtualServerManager::FailToMergeAllVirtualServerException::what() const throw() { return ("VirtualServerManager: Fail to merge all virtual server"); }