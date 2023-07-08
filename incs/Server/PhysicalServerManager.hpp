#ifndef PHYSICALSERVERMANAGER_HPP
# define PHYSICALSERVERMANAGER_HPP

# include "../../libs/shared_ptr/shared_ptr.hpp"
# include "../Config/Config.hpp"
# include "../Config/ServerElement.hpp"
# include "PhysicalServer.hpp"
# include <vector>
# include <map>

class PhysicalServerManager {
	public:
		typedef int										Port;
		typedef std::string								Ip;
		typedef ft::shared_ptr<PhysicalServer>			PhysicalServerPtr;
		typedef std::vector<PhysicalServerPtr>			PhysicalServerPtrVector;
		typedef std::map<Port, PhysicalServerPtrVector>	PortMap;

	private:
		PortMap	_port_map;

	public:
		PhysicalServerManager(void);
		~PhysicalServerManager(void);
		PhysicalServerManager(const PhysicalServerManager &ref);
		PhysicalServerManager	&operator=(const PhysicalServerManager &rhs);
	
	public:
		bool	build(const Config &config) throw(std::exception);
		bool	run(void) throw(std::exception);
	
	private:
		bool	_buildPhysicalServerVector(const ft::shared_ptr<ServerElement> &element) throw(std::exception);
		bool	_buildSocket(void) throw(std::exception);
		bool	_hasServerWithWildCardIp(const PortMap::const_iterator &it) const;
		bool	_mergeAllPhysicalServer(const PortMap::iterator &it) throw(std::exception);

	private:
		static PhysicalServerPtr	_findPhysicalServer(const PortMap::const_iterator &it, const Ip &ip);
	
	public:
		class FailToBuildException: public std::exception {
			public:
				virtual const char* what() const throw();
		};
		class FailToRunException: public std::exception {
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
		class FailToBuildPhysicalServerVectorException: public std::exception {
			public:
				virtual const char* what() const throw();
		};
		class FailToBuildSocketException: public std::exception {
			public:
				virtual const char* what() const throw();
		};

	friend std::ostream	&operator<<(std::ostream &os, const PhysicalServerManager &physical_server_manager);
};

#endif