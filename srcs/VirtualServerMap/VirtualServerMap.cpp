# include "../../incs/VirtualServerMap/VirtualServerMap.hpp"

VirtualServerMap::VirtualServerMap() {}

static bool _is_wildCard(const std::string &string) {
	if (string.empty() || string == "0.0.0.0") return true;
	else return false;
}

void VirtualServerMap::addOrUpdateServer(const KeyType& key, const std::vector<std::string>& serverNames, Server* currentServer) {
        iterator it = _listenSocket.find(key);
        if (it != _listenSocket.end()) {
            const std::string& entryIP = it->first.second;
            if (entryIP == key.second || 
			_is_wildCard(entryIP) || 
			_is_wildCard(key.second)
			) {
                _listenSocket[key][serverNames] = currentServer;
            }
        } else {
            addKey(key, serverNames, currentServer);
        }
    }

void VirtualServerMap::addKey(const KeyType& key, const std::vector<std::string>& serverNames, Server* currentServer) {
        _listenSocket[key] = std::map<std::vector<std::string>, Server*>();
        _listenSocket[key][serverNames] = currentServer;
    }

const VirtualServerMap::TargetMap& VirtualServerMap::getVertualServerMap(const KeyType& key) {
        return _listenSocket[key];
    }