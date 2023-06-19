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
		typedef std::map<Port, ft::shared_ptr<IpMap> >			PortMap;

	private:
		static const int	MAX_SERVERS = 8;

	private:
		PortMap	_portMap;

	public:
		PhysicalServerManager(void);
		~PhysicalServerManager(void);
	
	public:
		bool	build(const Config &config_map) throw(std::exception);
		bool	run(void) throw(std::exception);

	private:
		bool	_initPhysicalServers(const Config &config_map) throw(std::exception);
		bool	_mergeWildCardIpMaps(void) throw(std::exception);
		bool	_buildAllPhysicalServers(void) throw(std::exception) throw(std::exception);
		bool	_registerAllListeningEvents(void) throw(std::exception) throw(std::exception);

	private:
		int 							_initPort(const std::string &listen) throw(std::exception);
		std::string 					_initIp(const std::string &listen) throw(std::exception);
		ft::shared_ptr<PhysicalServer>	_initPhysicalServer(void) const;
		bool							_insertPhysicalServer(const Port &port, const Ip &ip, const ft::shared_ptr<PhysicalServer> &physicalServer) throw(std::exception);
		bool							_mergeIpMapsByPort(const PortMap::const_iterator &portIt);
	

	private:
		static bool	_portIsValid(const Port &port);
		static bool	_ipIsValid(const Ip &ip);
		static bool	_wildCardIpExists(const PortMap::const_iterator &portIt);
		static bool	_ipIsWildCard(const std::string &ip);
	
	public:
		ft::shared_ptr<PhysicalServer>	findPhysicalServer(const int port, const std::string &ip) const;

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