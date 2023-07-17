#ifndef PHYSICALSERVER_HPP
# define PHYSICALSERVER_HPP

# include "../../libs/shared_ptr/shared_ptr.hpp"
# include "../Config/ServerElement.hpp"
# include "../Channel/Socket.hpp"
# include "VirtualServerManager.hpp"
# include "VirtualServer.hpp"
# include <iostream>

class PhysicalServer {
	public:
		typedef int										Port;
		typedef std::string								Ip;
		typedef std::string								ServerName;
		typedef ft::shared_ptr<Socket>					SocketPtr;
		typedef ft::shared_ptr<ServerElement>			ServerElementPtr;
		typedef ft::shared_ptr<VirtualServer>			VirtualServerPtr;
		typedef ft::shared_ptr<PhysicalServer>			PhysicalServerPtr;
		typedef ft::shared_ptr<VirtualServerManager>	VirtualServerManagerPtr;

	private:
		Ip						_ip; // socket ip
		SocketPtr				_socket_ptr;
		VirtualServerManagerPtr	_virtual_server_manager_ptr;
	
	public:
		PhysicalServer(void);
		~PhysicalServer(void);
		PhysicalServer(const PhysicalServer &ref);
		PhysicalServer	&operator=(const PhysicalServer &rhs);
	
	public:
		bool	build(const Ip &ip, const ServerElementPtr &element) throw(std::exception);
		bool	buildSocket(const Port &port) throw(std::exception);
		bool	run(void) throw(std::exception);
		bool	hasServerWithWildCardIp(void) const;
		bool	mergeAllVirtualServer(const PhysicalServer &other) throw(std::exception);
	public:
		VirtualServerPtr	findVirtualServer(const Ip &ip) const;
		ft::shared_ptr<VirtualServerManager>	getVirtualServerManager(void) const;
		SocketPtr			getSocket(void) const;
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
		class FailToBuildSocketException: public std::exception {
			public:
				virtual const char* what() const throw();
		};

	friend std::ostream	&operator<<(std::ostream &os, const PhysicalServer &physical_server);
};

#endif