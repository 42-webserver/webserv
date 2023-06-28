#ifndef PHYSICALSERVERMANAGER_HPP
# define PHYSICALSERVERMANAGER_HPP

# include "../../libs/shared_ptr/shared_ptr.hpp"
# include "PhysicalServer.hpp"
# include <vector>
# include <map>

class PhysicalServerManager {
	public:
		typedef int												Port;
		typedef std::string										Ip;
		typedef std::vector< ft::shared_ptr< PhysicalServer > >	PhysicalServerVector;
		typedef std::map< Port, PhysicalServerVector >			PortMap;

	private:
		PortMap	_port_map;

	public:
		PhysicalServerManager(void);
		~PhysicalServerManager(void);
	
	private:
		PhysicalServerManager(const PhysicalServerManager &ref);
		PhysicalServerManager	&operator=(const PhysicalServerManager &rhs);
	
	public:
		bool	build(const Config &config_map) throw(std::exception);
		bool	run(void) throw(std::exception);
	
	public:
		ft::shared_ptr<PhysicalServer>	getPhysicalServer(const Port &port, const Ip &ip) const throw(std::exception);
	
	
	private:
		Port	_parsePort(const std::string &listen) throw(std::exception);
		Ip		_parseIp(const std::string &listen) throw(std::exception);
		bool	_hasServerWithWildCardIp(const PortMap::const_iterator &it) const;
		bool	_mergeAllPhysicalServer(const PortMap::iterator &it) throw(std::exception);
		ft::shared_ptr<PhysicalServer>	_getPhysicalServer(const Ip &ip) const;

	private:
		static bool	_portIsValid(const Port &port);
		static bool	_ipIsValid(const Ip &ip);
	
	public:
		class FailToBuildException: public std::exception {
			public:
				virtual const char* what() const throw();
		};
		class FailToRunException: public std::exception {
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
		class TooManyServerException: public std::exception {
			public:
				virtual const char* what() const throw();
		};
		class FailToMergeAllPhysicalServerException: public std::exception {
			public:
				virtual const char* what() const throw();
		};
};

#endif