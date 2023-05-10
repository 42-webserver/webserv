#ifndef SERVER_HPP
# define SERVER_HPP

# include "../../libs/shared_ptr/incs/shared_ptr.hpp"
# include "../data/Data.hpp"
# include "../config/Config.hpp"
# include <string>
# include <vector>
# include <iostream>
# include <map>

class Server: public Data {
	// Member variables
	private:
		const int						_port;
		const std::vector<std::string>	_names;
		const std::string				_root;
		const std::vector<std::string>	_indexes;
		const std::string				_default_error_page;
		const int						_client_max_body_size;
		const std::string				_cgi_pass;
		//std::string					_cgi_extension;
		//std::string					_cgi_path;
		//TODO: routing rules (location, redirect)

	// Static validators
	private:
		static bool						_is_valid_port(int port);
		static bool						_is_valid_client_max_body_size(int client_max_body_size);

	// Constructor & Destructor
	public:
		Server(const Config::map &config_map) throw(std::exception);
		virtual	~Server(void);
	
	public:
		int	accept(void) const;
	
	// Getters
	public:
		const std::string				&getDefaultErrorPage(void) const;
		int								getPort(void) const;
		const std::vector<std::string>	&getNames(void) const;
		int								getClientMaxBodySize(void) const;
		const std::string				&getRoot(void) const;
		const std::vector<std::string>	&getIndexes(void) const;
		const std::string				&getCgiPass(void) const;
		//const std::string				&getCgiExtension(void) const;
		//const std::string				&getCgiPath(void) const;
	
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