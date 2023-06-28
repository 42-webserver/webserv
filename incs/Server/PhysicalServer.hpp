#ifndef PHYSICALSERVER_HPP
# define PHYSICALSERVER_HPP

# include "../../libs/shared_ptr/shared_ptr.hpp"
# include "../Config/Config.hpp"
# include "../Socket/Socket.hpp"
# include "VirtualServerManager.hpp"

class PhysicalServer {
	public:
		typedef int								Port;
		typedef std::string						Ip;
		typedef std::string						ServerName;

	private:
		ft::shared_ptr<Socket>					_socket;
		ft::shared_ptr<VirtualServerManager>	_virtual_server_manager;
	
	public:
		PhysicalServer(void);
		~PhysicalServer(void);
		PhysicalServer(const PhysicalServer &ref);
		PhysicalServer	&operator=(const PhysicalServer &rhs);
	
	public:
		bool	build(const Ip &ip, const Config::map &config_map) throw(std::exception);
		bool	run(void) throw(std::exception);
		bool	hasServerWithWildCardIp(void) const;
		bool	mergeAllVirtualServer(const ft::shared_ptr<PhysicalServer> &other) throw(std::exception);
	
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
		class FailToMergeAllVirtualServerException: public std::exception {
			public:
				virtual const char* what() const throw();
		};

	friend std::ostream	&operator<<(std::ostream &os, const PhysicalServer &physical_server);
};

#endif