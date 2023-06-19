#ifndef VIRUTALSERVERMANAGER_HPP
# define VIRUTALSERVERMANAGER_HPP

# include "./VirtualServer.hpp"
# include <sstream>

class VirtualServerManager {
public:
    typedef std::string serverName;
    typedef std::string ip;
    typedef std::map<serverName, ft::shared_ptr<VirtualServer> > domainMap;
    typedef std::map<ip, ft::shared_ptr<VirtualServer> > serverMap;
    typedef std::map<std::string, std::string> hostsMap;

private:
    domainMap _domainMap;
    serverMap _servers;
    static hostsMap hostsFromFile;  // static member variable to store hosts from /etc/hosts
    int _port;
public:
    VirtualServerManager(int port);
    ~VirtualServerManager();
    void buildFromPhysicalServerManager(const serverMap& servers, const domainMap& domainMap);
    ft::shared_ptr<VirtualServer> find(const std::string& host);
    void parseHostsFile();  // function to parse /etc/hosts file and populate hostsFromFile
    int getListenPort() const;
    std::string getListenIP() const;
private:
    bool isDomainFormat(const std::string& host);
    bool isIPFormat(const std::string& host);
    bool isInEtcHosts(const std::string& host);
};


#endif