#ifndef SERVER_HPP
# define SERVER_HPP

# include "../../libs/shared_ptr/shared_ptr.hpp"
# include "../Config/Config.hpp"
# include "../Socket/Socket.hpp"
# include "Location.hpp"
# include <string>
# include <vector>
# include <iostream>
# include <map>

class Server {
	// Member variables
	private:
		const int						_port;
		const std::vector<std::string>	_names;
		const std::string				_root;
		const std::vector<std::string>	_indexes;
		const std::string				_default_error_page;
		const int						_client_max_body_size;
		const std::vector<Location>		_locations;

	// Static validators
	private:
		static bool						isValidPort(int port);
		static bool						isValidClientMaxBodySize(int client_max_body_size);
		static std::vector<Location>	initLocations(const Config::map &config_map);

	private:
		ft::shared_ptr<Socket>			_socket;
	
	// Constructor & Destructor
	public:
		Server(const Config::map &config_map) throw(std::exception);
		virtual	~Server(void);
	
	// Getters
	public:
		const std::string				&getDefaultErrorPage(void) const;
		int								getPort(void) const;
		const std::vector<std::string>	&getNames(void) const;
		int								getClientMaxBodySize(void) const;
		const std::string				&getRoot(void) const;
		const std::vector<std::string>	&getIndexes(void) const;
		const std::vector<Location>		&getLocations(void) const;
		const ft::shared_ptr<Socket>	&getSocket(void) const;
	
	//Operator
	public:
		bool operator<(const Server& other) const;

	// Exception
	public:
		class InvalidPortException: public std::exception {
			public:
				virtual const char *what() const throw();
		};
		class InvalidClientMaxBodySizeException: public std::exception {
			public:
				virtual const char *what() const throw();
		};
		class FailToInitializeLocationException: public std::exception {
			public:
				virtual const char *what() const throw();
		};
		class FailToCreateSocketException: public std::exception {
			public:
				virtual const char *what() const throw();
		};
		class FailToRunException: public std::exception {
			public:
				virtual const char *what() const throw();
		};
};

std::ostream	&operator<<(std::ostream &os, const Server &rhs);

#endif