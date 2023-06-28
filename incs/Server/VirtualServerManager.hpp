#ifndef VIRUTALSERVERMANAGER_HPP
# define VIRUTALSERVERMANAGER_HPP

# include "../../libs/shared_ptr/shared_ptr.hpp"
# include "../Config/Config.hpp"
# include "VirtualServer.hpp"
# include <string>
# include <vector>
# include <map>

class VirtualServerManager {
	public:
		typedef std::string														Ip;
		typedef std::vector< std::pair< Ip, ft::shared_ptr< VirtualServer > > >	VirtualServerVector;
		typedef std::string														ServerName;
		typedef std::map<ServerName, ft::shared_ptr<VirtualServer> >			ServerNameMap;

	private:
		VirtualServerVector		_virtual_server_vector;
		ServerNameMap 			_server_name_map;

	public:
		VirtualServerManager(void);
		~VirtualServerManager(void);
		VirtualServerManager(const VirtualServerManager &ref);
		VirtualServerManager	&operator=(const VirtualServerManager &rhs);

	public:
		bool	build(const Ip &ip, const Config::map &config_map) throw(std::exception);
		bool	run(void) throw(std::exception);
		bool	hasServerWithWildCardIp(void) const;
		bool	mergeAllVirtualServer(const ft::shared_ptr<VirtualServerManager> &other) throw(std::exception);
	
	public:
		ft::shared_ptr<VirtualServer>	findVirtualServerByIp(const Ip &ip) const;
		ft::shared_ptr<VirtualServer>	findVirtualServerByName(const ServerName &server_name) const;
	
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
		class FailToMergeAllVirtualServerException: public std::exception {
			public:
				virtual const char* what() const throw();
		};
};


#endif