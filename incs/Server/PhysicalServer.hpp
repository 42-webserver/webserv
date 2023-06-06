#ifndef PHYSICAL_SERVER_HPP
# define PHYSICAL_SERVER_HPP

# include "../../libs/shared_ptr/shared_ptr.hpp"
# include "../Config/Config.hpp"
# include "VirtualServer.hpp"
# include <string>
# include <map>

class PhysicalServer {
	public:
		typedef std::map<std::string, ft::shared_ptr<VirtualServer> > VirtualServerMap;

	private:
		std::string				_host;
		int						_port;
		ft::shared_ptr<Socket>	_socket;
		VirtualServerMap		_virtual_server_map;

	public:
		PhysicalServer(void);
		PhysicalServer(const std::string &host, const int port) throw(std::exception);
		~PhysicalServer(void);
	
	private:
		PhysicalServer(const PhysicalServer &ref);
		PhysicalServer	&operator=(const PhysicalServer &rhs);

	private:
		static ft::shared_ptr<Socket>	initSocket(int port) throw(std::exception);

		static bool						hostIsValid(const std::string &host);
		static bool						portIsValid(int port);
	
	public:
		void							addVirtualServer(const Config::map &config_map) throw(std::exception);
	
	public:
		ft::shared_ptr<Socket>			getSocket(void) const;
		const VirtualServerMap			*getVirtualServerMap(void) const;
	public:
		class InvalidHostException: public std::exception {
			public:
				virtual const char *what() const throw();
		};
		class InvalidPortException: public std::exception {
			public:
				virtual const char *what() const throw();
		};
		class FailToCreateSocketException: public std::exception {
			public:
				virtual const char *what() const throw();
		};
		class DuplicatedVirtualServerException: public std::exception {
			public:
				virtual const char *what() const throw();
		};
};

#endif