#include "../../incs/Server/VirtualServerManager.hpp"
#include "../../incs/Log/Logger.hpp"
#include <fstream>
#include <sstream>

const VirtualServerManager::EtcHostsMap 			VirtualServerManager::ETC_HOSTS_MAP = VirtualServerManager::_initEtcHostsMap();
VirtualServerManager::EtcHostsMap 					VirtualServerManager::_initEtcHostsMap(void) {
	std::map< VirtualServerManager::ServerName, VirtualServerManager::Ip >	ret;
	std::ifstream															infile("/etc/hosts");
	std::string																line;

	if (infile.is_open()) {
		while (std::getline(infile, line)) {
			std::istringstream	iss;
			Ip					ip;
			ServerName			server_name;

			if (line[0] == '#') {
				continue ;
			}

			iss = std::istringstream(line);
			iss >> ip;
			while (iss >> server_name) {
				ret.insert(std::make_pair(server_name, ip));
			}
		}
		infile.close();
	}
	return (ret);
}

VirtualServerManager::VirtualServerManager(void): _ip_map(IpMap()), _server_name_map(ServerNameMap()) {}
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

ft::shared_ptr<VirtualServer>	VirtualServerManager::findVirtualServer(const Host &host) const throw(std::exception) {
	ft::shared_ptr<VirtualServer>	ret = ft::shared_ptr<VirtualServer>(NULL);
	const Host						trimmed = VirtualServerManager::_trimHost(host);

	if (VirtualServerManager::_isIpFormat(trimmed)) {
		ret = this->_findVirtualServerByIp(trimmed);
	} else if (VirtualServerManager::_isServerNameFormat(trimmed)) {
		ret = this->_findVirtualServerByName(trimmed);
	} else {
		throw (InvalidHostFormatException());
	}
	if (ret.get() == NULL) {
		// Set to Default Server
	}
	return (ret);
}

VirtualServerManager::Host		VirtualServerManager::_trimHost(const Host &host) const {
	Host	ret = host;

	if (host.find(":") != std::string::npos) {
		ret = host.substr(0, host.find(":"));
	}
	return (ret);
}
bool							VirtualServerManager::_isIpFormat(const Host &host) const {
	for (size_t i=0; i<host.size(); i++) {
		if (host[i] == '.') {
			continue;
		}
		if (host[i] < '0' || '9' < host[i]) {
			return (false);
		}
	}
	return (true);
}
bool							VirtualServerManager::_isServerNameFormat(const Host &host) const {
	for (size_t i=0; i<host.size(); i++) {
		if (host[i] == '.') {
			continue;
		}
		if (host[i] < 'a' || 'z' < host[i]) {
			return (false);
		}
	}
	return (true);
}
ft::shared_ptr<VirtualServer>	VirtualServerManager::_findVirtualServerByIp(const Ip &ip) const {
	ft::shared_ptr<VirtualServer>	ret = ft::shared_ptr<VirtualServer>(NULL);
	IpMap::const_iterator			it = this->_ip_map.find(ip);

	if (it != this->_ip_map.end()) {
		ret = it->second;
	}
	return (ret);
}
ft::shared_ptr<VirtualServer>	VirtualServerManager::_findVirtualServerByName(const ServerName &server_name) const {
	ft::shared_ptr<VirtualServer>	ret = ft::shared_ptr<VirtualServer>(NULL);

	ret = VirtualServerManager::_findVirtualServerByServerName(server_name);
	if (ret.get() == NULL) {
		ret = VirtualServerManager::_findVirtualServerByEtcHosts(server_name);
	}
	return (ret);
}
ft::shared_ptr<VirtualServer>	VirtualServerManager::_findVirtualServerByReservedServerName(const ServerName &server_name) const {
	return (this->_findVirtualServerByIp(VirtualServerManager::RESERVED_SERVER_NAME_MAP.find(server_name)->second));
}
ft::shared_ptr<VirtualServer>	VirtualServerManager::_findVirtualServerByServerName(const ServerName &server_name) const {
	ft::shared_ptr<VirtualServer>	ret = ft::shared_ptr<VirtualServer>(NULL);
	ServerNameMap::const_iterator	it = this->_server_name_map.find(server_name);

	if (it != this->_server_name_map.end()) {
		ret = it->second;
	}
	return (ret);
}
ft::shared_ptr<VirtualServer>	VirtualServerManager::_findVirtualServerByEtcHosts(const ServerName &server_name) const {
	return (this->_findVirtualServerByIp(VirtualServerManager::ETC_HOSTS_MAP.find(server_name)->second));
}

const char	*VirtualServerManager::FailToBuildException::what() const throw() { return ("VirtualServerManager: Fail to build"); }
const char	*VirtualServerManager::DuplicatedServerNameException::what() const throw() { return ("VirtualServerManager: Duplicated server name"); }
const char	*VirtualServerManager::FailToMergeAllVirtualServerException::what() const throw() { return ("VirtualServerManager: Fail to merge all virtual server"); }
const char	*VirtualServerManager::InvalidHostFormatException::what() const throw() { return ("VirtualServerManager: Invalid host format"); }

std::ostream	&operator<<(std::ostream &os, const VirtualServerManager &virtual_server_manager) {
	os << "\t\t\t\t" << "VirtualServerManager:" << '\n';
	os << "\t\t\t\t\t" << "IpMap:" << '\n';
	for (VirtualServerManager::IpMap::const_iterator it=virtual_server_manager._ip_map.begin(); it!=virtual_server_manager._ip_map.end(); ++it) {
		os << "\t\t\t\t\t\t" << it->first << ":" << '\n';
		os << *(it->second);
	}
	os << "\t\t\t\t\t" << "ReservedServerNameMap:" << '\n';
	for (VirtualServerManager::ReservedServerNameMap::const_iterator it=VirtualServerManager::RESERVED_SERVER_NAME_MAP.begin(); it!=VirtualServerManager::RESERVED_SERVER_NAME_MAP.end(); ++it) {
		os << "\t\t\t\t\t\t" << it->second << ' ' << it->first << '\n';
	}
	os << "\t\t\t\t\t" << "ServerNameMap:" << '\n';
	for (VirtualServerManager::ServerNameMap::const_iterator it=virtual_server_manager._server_name_map.begin(); it!=virtual_server_manager._server_name_map.end(); ++it) {
		os << "\t\t\t\t\t\t" << it->first << ":" << '\n';
		os << *(it->second);
	}
	os << "\t\t\t\t\t" << "EtcHostsMap:" << '\n';
	for (VirtualServerManager::EtcHostsMap::const_iterator it=VirtualServerManager::ETC_HOSTS_MAP.begin(); it!=VirtualServerManager::ETC_HOSTS_MAP.end(); ++it) {
		os << "\t\t\t\t\t\t" << it->second << ' ' << it->first << '\n';
	}
	return (os);
}