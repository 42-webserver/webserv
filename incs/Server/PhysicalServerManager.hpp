#ifndef PHYSICALSERVERMANAGER_HPP
# define PHYSICALSERVERMANAGER_HPP

# include "../../libs/shared_ptr/shared_ptr.hpp"
# include "VirtualServerManager.hpp"
# include <string>
# include <map>

class PhysicalServerManager {
	public:
		typedef int			Port;
		typedef std::string	Ip;

	public:
		typedef VirtualServerManager 							PhysicalServer;
		typedef std::map<Ip, ft::shared_ptr<PhysicalServer> >	IpMap;
		typedef std::map<Port, ft::shared_ptr<IpMap> >			PortMap;

	private:
		static const int	MAX_SERVERS = 8;

	private:
		PortMap	_port_map;

	public:
		PhysicalServerManager(void);
		~PhysicalServerManager(void);
	
	public:
		bool	build(const Config &config_map) throw(std::exception);
		bool	run(void) throw(std::exception);
	
	public:
		ft::shared_ptr<PhysicalServer>	findPhysicalServer(const int port, const std::string &ip) const;

	private:
		bool	_initPhysicalServers(const Config &config_map) throw(std::exception);
		bool	_mergeWildCardIpMaps(void);
		bool	_buildAllPhysicalServers(void) throw(std::exception);
		bool	_registerAllListeningEvents(void) throw(std::exception);
		bool	_insertPhysicalServer(const Port &port, const Ip &ip, const ft::shared_ptr<PhysicalServer> &physicalServer);
		bool	_mergeIpMapsByPort(const PortMap::const_iterator &portIt);

	private:
		static int 			_parsePort(const std::string &listen) throw(std::exception);
		static std::string	_parseIp(const std::string &listen) throw(std::exception);
		static bool			_portIsValid(const Port &port);
		static bool			_ipIsValid(const Ip &ip);
		static bool			_wildCardIpExists(const PortMap::const_iterator &portIt);
		static bool			_ipIsWildCard(const std::string &ip);
	
	public:
		class FailToRunException: public std::exception {
			public:
				virtual const char* what() const throw();
		};
		class FailToBuildException: public std::exception {
			public:
				virtual const char* what() const throw();
		};
		class TooManyServerException: public std::exception {
			public:
				virtual const char* what() const throw();
		};
		class InvalidPortException: public std::exception {
			public:
				virtual const char* what() const throw();
		};
		class InvalidIpException: public std::exception {
			public:
				virtual const char* what() const throw();
		};
		class FailToInitPhysicalServersException: public std::exception {
			public:
				virtual const char* what() const throw();
		};
		class FailToBuildAllPhysicalServersException: public std::exception {
			public:
				virtual const char* what() const throw();
		};
		class FailToRegisterAllListeningEventsException: public std::exception {
			public:
				virtual const char* what() const throw();
		};
};

#endif