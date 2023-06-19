#ifndef PHYSICALSERVERMANAGER_HPP
# define PHYSICALSERVERMANAGER_HPP

# include "VirtualServer.hpp"
# include "VirtualServerManager.hpp"
# include <sstream>

class PhysicalServerManager {
	public:
		typedef int			Port;
		typedef std::string	Ip;

	public:
		typedef VirtualServerManager 							PhysicalServer;
		typedef std::map<Ip, ft::shared_ptr<PhysicalServer> >	IpMap;
		typedef std::map<Port, IpMap>							PortMap;

	private:
		static const int	MAX_SERVERS = 8;

	private:
		PortMap	_portMap;

	public:
		PhysicalServerManager(void);
		~PhysicalServerManager(void);
	
	public:
		bool	run(const Config &config_map) throw(std::exception);

	private:
		bool	_init(const Config &config_map) throw(std::exception);
		bool	_build(void) throw(std::exception);

	private:
		int 		_initPort(const std::string &listen);
		std::string _initIp(const std::string &listen);
	
	private:
		ft::shared_ptr<PhysicalServer>	_initPhysicalServer(void) const;
		bool							_insertPhysicalServer(const Port port, const Ip &ip, ft::shared_ptr<PhysicalServer> physicalServer) throw(std::exception);
		bool							_insertVirtualServerToPhysicalServer(ft::shared_ptr<PhysicalServer> physicalServer, const Config::map &config_map) throw(std::exception);
		bool							_mergeIpMapByPort(const PortMap::const_iterator portIt) throw(std::exception);
	
	private:
		bool	_wildCardIpExists(const PortMap::const_iterator portIt) const;
		bool	_ipIsWildCard(const std::string &ip) const;
	
	public:
		ft::shared_ptr<PhysicalServer>	findPhysicalServer(const int port, const std::string &ip) const;

	public:
		class FailToRun: public std::exception {
			public:
				virtual const char* what() const throw();
		};
		class TooManyServerException: public std::exception {
			public:
				virtual const char* what() const throw();
		};
		class FailToInit: public std::exception {
			public:
				virtual const char* what() const throw();
		};
		class FailToBuild: public std::exception {
			public:
				virtual const char* what() const throw();
		};
};

#endif