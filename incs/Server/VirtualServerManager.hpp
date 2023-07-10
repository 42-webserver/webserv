#ifndef VIRUTALSERVERMANAGER_HPP
# define VIRUTALSERVERMANAGER_HPP

# include "../../libs/shared_ptr/shared_ptr.hpp"
# include "../Config/ServerElement.hpp"
# include "../Config/ServerNameElement.hpp"
# include "VirtualServer.hpp"
# include <iostream>
# include <string>
# include <map>

class VirtualServerManager {
	public:
		typedef std::string									Host;
		typedef std::string									Ip;
		typedef std::string									ServerName;
		typedef ft::shared_ptr<ServerElement>				ServerElementPtr;
		typedef ft::shared_ptr<ServerNameElement>			ServerNameElementPtr;
		typedef ft::shared_ptr<VirtualServer>				VirtualServerPtr;
		typedef std::map< Ip, VirtualServerPtr >			IpMap;
		typedef std::map< ServerName, VirtualServerPtr >	ServerNameMap;
	
	private:
		typedef std::map<ServerName, Ip>	EtcHostsMap;
		static const EtcHostsMap			ETC_HOSTS_MAP;
		static EtcHostsMap					_initEtcHostsMap(void);
	
	private:
		Ip				_default_server_ip;
		IpMap			_ip_map;
		ServerNameMap 	_server_name_map;

	public:
		VirtualServerManager(void);
		~VirtualServerManager(void);
		VirtualServerManager(const VirtualServerManager &ref);
		VirtualServerManager	&operator=(const VirtualServerManager &rhs);

	public:
		bool	build(const Ip &ip, const ServerElementPtr &element) throw(std::exception);
		bool	hasServerWithWildCardIp(void) const;
		bool	mergeAllVirtualServer(const VirtualServerManager &other) throw(std::exception);
	
	public:
		VirtualServerPtr	findVirtualServer(const Host &host) const throw(std::exception);
	
	private:
		VirtualServerPtr	_findVirtualServerByIp(const Ip &ip) const;
		VirtualServerPtr	_findVirtualServerByName(const ServerName &server_name) const;
		VirtualServerPtr	_findVirtualServerByServerName(const ServerName &server_name) const;
		VirtualServerPtr	_findVirtualServerByEtcHosts(const ServerName &server_name) const;
	
	private:
		static Host			_trimHost(const Host &host);
		static bool			_isIpFormat(const Host &host);
		static bool			_isServerNameFormat(const Host &host);
	
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