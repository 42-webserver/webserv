#include "../../../incs/Server/VirtualServerManager.hpp"
#include "../../../incs/Config/Config.hpp"

bool isDomainFormat(const std::string& host) {
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

bool isIPFormat(const std::string& host) {
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
std::map<std::string, std::string>  parseHostsFile() {
 
    std::ifstream hostsFile("/etc/hosts");
	std::map<std::string, std::string> hostsFromFile;
    if (!hostsFile.is_open()) {
		return hostsFromFile;
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
	return hostsFromFile;
}
int main(int argc, char **argv) {
	// Config	config;

	std::cout << isDomainFormat("www.118.0.1") << std::endl;
	std::cout << isDomainFormat("www.118.0.com") << std::endl;
	std::cout << isIPFormat("-111.119.111.0") << std::endl;
	std::cout << isIPFormat("119.0.0.0") << std::endl;
	std::cout << isIPFormat("0.0.0.0") << std::endl;
	std::cout << isIPFormat("0.0.0.0.0") << std::endl;
	// this allow
	std::cout << isIPFormat("02.0.0.0") << std::endl;
	std::cout << isIPFormat("0.0.0.256") << std::endl;

	std::map<std::string, std::string> hostsFromFile = parseHostsFile();
	for (std::map<std::string, std::string>::iterator it = hostsFromFile.begin(); it != hostsFromFile.end(); ++it) {
		std::cout << it->first << " " << it->second << std::endl;
	}

}