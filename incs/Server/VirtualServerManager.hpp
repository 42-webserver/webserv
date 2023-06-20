#ifndef VIRUTALSERVERMANAGER_HPP
# define VIRUTALSERVERMANAGER_HPP

# include "../../libs/shared_ptr/shared_ptr.hpp"
# include "../Config/Config.hpp"
# include "../Socket/Socket.hpp"
# include "VirtualServer.hpp"
# include <string>
# include <map>

class VirtualServerManager {
	public:
		typedef std::string												serverName;
		typedef std::map<serverName, ft::shared_ptr<VirtualServer> >	DomainMap;

	private:
		DomainMap 				_domain_map;
		ft::shared_ptr<Socket>	_socket;
		//static hostsMap hostsFromFile;  // static member variable to store hosts from /etc/hosts

	public:
		VirtualServerManager(void);
		~VirtualServerManager(void);

	public:
		bool	build(const int port, const std::string &ip) throw(std::exception);
		bool	run(void) throw(std::exception);
	
	public:
		ft::shared_ptr<VirtualServer>	findVirtualServer(const std::string &domain) const;
	
	public:
		bool	addVirtualServers(const Config::map &config_map) throw(std::exception);
		bool	mergeVirtualServers(const ft::shared_ptr<VirtualServerManager> &virtualServerManager) throw(std::exception);
		//void parseHostsFile(void);  // function to parse /etc/hosts file and populate hostsFromFile

	private:
		const DomainMap	&getDomainMap(void) const;

	//private:
		//bool isDomainFormat(const std::string& host);
		//bool isIPFormat(const std::string& host);
		//bool isInEtcHosts(const std::string& host);

	public:
		class FailToBuildException: public std::exception {
			public:
				virtual const char* what() const throw();
		};
		class FailToRunException: public std::exception {
			public:
				virtual const char* what() const throw();
		};
		class DuplicatedServerNameException: public std::exception {
			public:
				virtual const char* what() const throw();
		};
};


#endif