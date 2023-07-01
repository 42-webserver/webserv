#ifndef VIRUTALSERVERMANAGER_HPP
# define VIRUTALSERVERMANAGER_HPP

# include "../../libs/shared_ptr/shared_ptr.hpp"
# include "../Config/Config.hpp"
# include "VirtualServer.hpp"
# include <string>
# include <map>

class VirtualServerManager {
	public:
		typedef std::string												Host;
		typedef std::string												Ip;
		typedef std::string												ServerName;
		typedef std::map< Ip, ft::shared_ptr< VirtualServer > >			IpMap;
		typedef std::map< ServerName, ft::shared_ptr< VirtualServer > >	ServerNameMap;
	
	private:
		typedef std::map< ServerName, Ip >	EtcHostsMap;
		static const EtcHostsMap			ETC_HOSTS_MAP;
		static EtcHostsMap					_initEtcHostsMap(void);
	
	private:
		IpMap			_ip_map;
		ServerNameMap 	_server_name_map;

	public:
		VirtualServerManager(void);
		~VirtualServerManager(void);
		VirtualServerManager(const VirtualServerManager &ref);
		VirtualServerManager	&operator=(const VirtualServerManager &rhs);

	public:
		bool	build(const Ip &ip, const Config::map &config_map) throw(std::exception);
		bool	hasServerWithWildCardIp(void) const;
		bool	mergeAllVirtualServer(const ft::shared_ptr<VirtualServerManager> &other) throw(std::exception);
	
	public:
		ft::shared_ptr<VirtualServer>	findVirtualServer(const Host &host) const throw(std::exception);
	
	private:
		Host							_trimHost(const Host &host) const;
		bool							_isIpFormat(const Host &host) const;
		bool							_isServerNameFormat(const Host &host) const;
		ft::shared_ptr<VirtualServer>	_findVirtualServerByIp(const Ip &ip) const;
		ft::shared_ptr<VirtualServer>	_findVirtualServerByName(const ServerName &server_name) const;
		ft::shared_ptr<VirtualServer>	_findVirtualServerByReservedServerName(const ServerName &server_name) const;
		ft::shared_ptr<VirtualServer>	_findVirtualServerByServerName(const ServerName &server_name) const;
		ft::shared_ptr<VirtualServer>	_findVirtualServerByEtcHosts(const ServerName &server_name) const;
	
	public:
		class FailToBuildException: public std::exception {
			public:
				virtual const char* what() const throw();
		};
		class DuplicatedServerNameException: public std::exception {
			public:
				virtual const char* what() const throw();
		};
		class FailToMergeAllVirtualServerException: public std::exception {
			public:
				virtual const char* what() const throw();
	 	};
		class InvalidHostFormatException: public std::exception {
			public:
				virtual const char* what() const throw();
		};

	friend std::ostream	&operator<<(std::ostream &os, const VirtualServerManager &virtual_server_manager);
};

#endif