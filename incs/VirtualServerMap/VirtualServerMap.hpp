#ifndef VIRTUALSERVERMAP_HPP
# define VIRTUALSERVERMAP_HPP

# include <iostream>
# include <map>
# include <vector>
# include "../Server/Server.hpp"

class VirtualServerMap {
public:
    typedef std::pair<int, std::string> KeyType;
    typedef std::map<std::vector<std::string>, Server*> TargetMap;
    typedef std::map<KeyType, std::map<std::vector<std::string>, Server*> >::iterator iterator;

private:
    std::map<KeyType, std::map<std::vector<std::string>, Server*> > _listenSocket;

public:
    VirtualServerMap();

    void addOrUpdateServer(const KeyType& key, const std::vector<std::string>& serverNames, Server* currentServer);

    void addKey(const KeyType& key, const std::vector<std::string>& serverNames, Server* currentServer);
   const TargetMap* getVertualServerMap(const KeyType& key);
};

#endif