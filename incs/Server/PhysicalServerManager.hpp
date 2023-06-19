#ifndef PHYSICALSERVERMANAGER_HPP
# define PHYSICALSERVERMANAGER_HPP

# include "./VirtualServer.hpp"
# include <sstream>
# include "VirtualServerManager.hpp"

class PhysicalServerManager {
public:
    typedef int port;
    typedef std::string ip;
    typedef std::string serverName;
    typedef std::map<ip, ft::shared_ptr<VirtualServer> >::iterator ip_iterator;

private:
		static const int	MAX_SERVERS = 8;
private:
    std::map<port, std::multimap<ip, ft::shared_ptr<VirtualServer> > > _portMap;
    std::map<serverName, ft::shared_ptr<VirtualServer> > _domainMap;
    std::vector<VirtualServerManager> _VirtualServerManagers;
    std::vector<VirtualServerManager>::iterator _vsmit;

public:
    PhysicalServerManager();
    VirtualServerManager getCurrentVirtualServerManager();
    int getVirtualServerManagerCount();
    void run(const Config& config_map) throw(std::exception);
private:
    std::string _initHost(const std::string& listen);
    int _initPort(const std::string& listen);
    void insertVirtualServer(port, ip, const Config::map& config_map, const std::vector<std::string>& server_names);
    void buildAllVirtualServerManagers();
    void insertAllVirtualServers(const Config& config_map);
    bool checkWildcardIP(const std::multimap<ip, ft::shared_ptr<VirtualServer> >& ipMultimap);
    std::map<ip, ft::shared_ptr<VirtualServer> > buildIPMap(const std::multimap<ip, ft::shared_ptr<VirtualServer> >& ipMultimap, bool isWildcard, ip targetIP);
    std::map<serverName, ft::shared_ptr<VirtualServer> > buildDomainMap(const std::map<ip, ft::shared_ptr<VirtualServer> >& ipMap);
    void createAndStoreVirtualServerManager(std::map<ip, ft::shared_ptr<VirtualServer> >& ipMap, std::map<serverName, 
    ft::shared_ptr<VirtualServer> >& domainMap, int port);
public:
    class FailToConstructException: public std::exception {
        public:
            virtual const char* what() const throw();
    };
    class DuplicateServerException: public std::exception {
        public:
            virtual const char* what() const throw();
    };
};





#endif