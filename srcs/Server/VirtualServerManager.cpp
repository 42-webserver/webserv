#include "../../incs/Server/VirtualServerManager.hpp"
#include <fstream>
#include <sstream>

VirtualServerManager::hostsMap VirtualServerManager::hostsFromFile = VirtualServerManager::hostsMap();

VirtualServerManager::VirtualServerManager(int port) : _port(port){ 
    parseHostsFile();
}
VirtualServerManager::~VirtualServerManager() {}
// VirtualServerManager.cpp
void VirtualServerManager::buildFromPhysicalServerManager(const serverMap& servers, const domainMap& domainMap) {
    _servers = servers;
    _domainMap = domainMap;
}


VirtualServerManager::ip VirtualServerManager::getListenIP() const {
    if (_servers.find("0.0.0.0") != _servers.end()) {
        return "0.0.0.0";
    } else {   
        return _servers.begin()->first;
    }
}

int VirtualServerManager::getListenPort() const {
    return _port;
}
ft::shared_ptr<VirtualServer> VirtualServerManager::find(const std::string& host ) {
    // std::string host = hostheader.substr(0, hostheader.find(':'));
    
    // local host <-here .
	if (isInEtcHosts(host)) {
		return _servers[hostsFromFile[host]];
	}
    else if (isDomainFormat(host)) {
        if (_domainMap.count(host) > 0) {
            return _domainMap[host];
        }
    } else if (isIPFormat(host)) {
        if (_servers.count(host) > 0) {
            return _servers[host];
        } else {
            std::map<VirtualServerManager::ip, 
			ft::shared_ptr<VirtualServer> >::const_iterator it = _servers.find("0.0.0.0");
            if (it != _servers.end()) {
                return it->second;
            }
        }
    } 

    return NULL;
}

       
bool VirtualServerManager::isDomainFormat(const std::string& host) {
    if (host.empty()) return false;
    
    // Check each character
    for (std::string::const_iterator it = host.begin(); it != host.end(); ++it) {
        if (!std::isalnum(*it) && *it != '-' && *it != '.') {
            return false; // invalid character found
        }
    }
    
    std::vector<std::string> parts;
    std::string::size_type pos = 0;
    std::string::size_type dotPos;
    // separate host by '.'
    while ((dotPos = host.find('.', pos)) != std::string::npos) {
        std::string part = host.substr(pos, dotPos - pos);
        if (part.empty() || !std::isalnum(part[0]) || !std::isalnum(part[part.size() - 1])) {
            return false; // each part must not be empty and must start/end with an alphanumeric character
        }
        parts.push_back(part);
        pos = dotPos + 1;
    }
    
    std::string lastPart = host.substr(pos);
    if (lastPart.empty() || !std::isalpha(lastPart[0]) || !std::isalnum(lastPart[lastPart.size() - 1])) {
        return false; // the last part (TLD) must not be empty, must start with an alphabetic character and end with an alphanumeric character
    }
    parts.push_back(lastPart);

    return true;
}

bool VirtualServerManager::isIPFormat(const std::string& host) {
    // Check if the host is in IP format
    std::vector<std::string> parts;
    std::string::size_type pos = 0;
    std::string::size_type dotPos;
    while ((dotPos = host.find('.', pos)) != std::string::npos) {
        parts.push_back(host.substr(pos, dotPos - pos));
        pos = dotPos + 1;
    }
    parts.push_back(host.substr(pos));

    if (parts.size() != 4) {
        return false;
    }

    for (std::vector<std::string>::const_iterator it = parts.begin(); it != parts.end(); ++it) {
        const std::string& part = *it;
        if (part.empty() || part.size() > 3) {
            return false;
        }
        // Reject parts that start with '0' but have more than one digit
        if (part[0] == '0' && part.size() > 1) {
            return false;
        }
        for (std::string::size_type i = 0; i < part.size(); ++i) {
            if (!std::isdigit(part[i])) {
                return false;
            }
        }
        int num = std::atoi(part.c_str());
        if (num < 0 || num > 255) {
            return false;
        }
    }

    return true;
}

void VirtualServerManager::parseHostsFile() {
    if (!hostsFromFile.empty()) {
        return;
    }

    std::ifstream hostsFile("/etc/hosts");
    if (!hostsFile.is_open()) {
        return;
    }

    std::string line;
    while (std::getline(hostsFile, line)) {
        if (line[0] == '#') {
            continue;
        }

        std::istringstream iss(line);
        std::string ip, host;
        
        if (!(iss >> ip >> host)) {
            break;
        }
        hostsFromFile[host] = ip;
    }

    hostsFile.close();
}

bool VirtualServerManager::isInEtcHosts(const std::string& host) {
    return hostsFromFile.count(host) > 0;
}

