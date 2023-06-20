#include "../../incs/Server/VirtualServerManager.hpp"
#include "../../incs/Log/Logger.hpp"
#include <fstream>
#include <sstream>

//VirtualServerManager::hostsMap VirtualServerManager::hostsFromFile = VirtualServerManager::hostsMap();

VirtualServerManager::VirtualServerManager(void) {
	//parseHostsFile();
}
VirtualServerManager::~VirtualServerManager(void) {}

bool	VirtualServerManager::build(const int port, const std::string &ip) throw(std::exception) {
	try {
		this->_socket = ft::shared_ptr<Socket>(new Socket());
		this->_socket->build(port, ip);
	} catch (std::exception &e) {
		Logger::getInstance().error(e.what());
		throw (VirtualServerManager::FailToBuildException());
	}

	return (true);
}
bool	VirtualServerManager::run(void) throw(std::exception) {
	try {
		this->_socket->run();
	} catch (std::exception &e) {
		Logger::getInstance().error(e.what());
		throw (VirtualServerManager::FailToRunException());
	}

	return (true);
}

//ft::shared_ptr<VirtualServer>	VirtualServerManager::findVirtualServer(const std::string &domain) const {
//	DomainMap::const_iterator	it;

//	if ((it = this->_domain_map.find(domain)) == this->_domain_map.end()) {
//		return (ft::shared_ptr<VirtualServer>(NULL));
//	}

//	return (it->second);
//}

//bool	registerListeningEvent() {
//	EventQueue		&eventQueue = EventQueue::getInstance();
//	EventFactory	&eventFactory = ListeningEventFactory::getInstance();

//	eventQueue.pushEvent(eventFactory.createEvent(EventDto()));
//}

bool	VirtualServerManager::addVirtualServers(const Config::map &config_map) throw(std::exception) {
	ft::shared_ptr<VirtualServer> virtual_server = ft::shared_ptr<VirtualServer>(new VirtualServer(config_map));

	for (size_t i=0; i<config_map.at(Config::KEYS[Config::KEY::SERVER_NAME]).size(); ++i) {
		if (this->_domain_map.count(config_map.at(Config::KEYS[Config::KEY::SERVER_NAME])[i]) > 0) {
			throw (VirtualServerManager::DuplicatedServerNameException());
		}

		this->_domain_map[config_map.at(Config::KEYS[Config::KEY::SERVER_NAME])[i]] = virtual_server;
	}

	return (true);
}
bool	VirtualServerManager::mergeVirtualServers(const ft::shared_ptr<VirtualServerManager> &virtualServerManager) throw(std::exception) {
	for (DomainMap::const_iterator it=virtualServerManager->getDomainMap().begin(); it!=virtualServerManager->getDomainMap().end(); ++it) {
		if (this->_domain_map.count(it->first) > 0) {
			throw (VirtualServerManager::DuplicatedServerNameException());
		}

		this->_domain_map[it->first] = it->second;
	}

	return (true);
}

const VirtualServerManager::DomainMap	&VirtualServerManager::getDomainMap(void) const { return (this->_domain_map); }

//ft::shared_ptr<VirtualServer> VirtualServerManager::find(const std::string& hostheader ) {
//	std::string host = hostheader.substr(0, hostheader.find(':'));
//	// local host <-here .
//	if (isInEtcHosts(host)) {
//		return _servers[hostsFromFile[host]];
//	}
//	else if (isDomainFormat(host)) {
//		if (_domain_map.count(host) > 0) {
//			return _domain_map[host];
//		}
//	} else if (isIPFormat(host)) {
//		if (_servers.count(host) > 0) {
//			return _servers[host];
//		} else {
//			std::map<VirtualServerManager::ip, 
//			ft::shared_ptr<VirtualServer> >::const_iterator it = _servers.find("0.0.0.0");
//			if (it != _servers.end()) {
//				return it->second;
//			}
//		}
//	} 

//	return NULL;
//}

	   
//bool VirtualServerManager::isDomainFormat(const std::string& host) {
//	if (host.empty()) return false;
	
//	// Check each character
//	for (std::string::const_iterator it = host.begin(); it != host.end(); ++it) {
//		if (!std::isalnum(*it) && *it != '-' && *it != '.') {
//			return false; // invalid character found
//		}
//	}
	
//	std::vector<std::string> parts;
//	std::string::size_type pos = 0;
//	std::string::size_type dotPos;
//	// separate host by '.'
//	while ((dotPos = host.find('.', pos)) != std::string::npos) {
//		std::string part = host.substr(pos, dotPos - pos);
//		if (part.empty() || !std::isalnum(part[0]) || !std::isalnum(part[part.size() - 1])) {
//			return false; // each part must not be empty and must start/end with an alphanumeric character
//		}
//		parts.push_back(part);
//		pos = dotPos + 1;
//	}
	
//	std::string lastPart = host.substr(pos);
//	if (lastPart.empty() || !std::isalpha(lastPart[0]) || !std::isalnum(lastPart[lastPart.size() - 1])) {
//		return false; // the last part (TLD) must not be empty, must start with an alphabetic character and end with an alphanumeric character
//	}
//	parts.push_back(lastPart);

//	return true;
//}

//bool VirtualServerManager::isIPFormat(const std::string& host) {
//	// Check if the host is in IP format
//	std::vector<std::string> parts;
//	std::string::size_type pos = 0;
//	std::string::size_type dotPos;
//	while ((dotPos = host.find('.', pos)) != std::string::npos) {
//		parts.push_back(host.substr(pos, dotPos - pos));
//		pos = dotPos + 1;
//	}
//	parts.push_back(host.substr(pos));

//	if (parts.size() != 4) {
//		return false;
//	}

//	for (std::vector<std::string>::const_iterator it = parts.begin(); it != parts.end(); ++it) {
//		const std::string& part = *it;
//		if (part.empty() || part.size() > 3) {
//			return false;
//		}
//		// Reject parts that start with '0' but have more than one digit
//		if (part[0] == '0' && part.size() > 1) {
//			return false;
//		}
//		for (std::string::size_type i = 0; i < part.size(); ++i) {
//			if (!std::isdigit(part[i])) {
//				return false;
//			}
//		}
//		int num = std::atoi(part.c_str());
//		if (num < 0 || num > 255) {
//			return false;
//		}
//	}

//	return true;
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