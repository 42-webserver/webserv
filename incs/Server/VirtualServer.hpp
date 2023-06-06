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

class VirtualServer {
	private:
		const std::string				_root;
		const std::vector<std::string>	_indexes;
		const std::string				_default_error_page;
		const int						_client_max_body_size;
		const std::vector<Location>		_locations;

	private:
		static std::string				_initRoot(const Config::map &config_map);
		static std::vector<std::string>	_initIndexes(const Config::map &config_map);
		static std::string				_initDefaultErrorPage(const Config::map &config_map);
		static int						_initClientMaxBodySize(const Config::map &config_map);
		static std::vector<Location>	_initLocations(const Config::map &config_map);

	//Custom will be added
		// static bool						_rootIsValid(const std::string &root);
		// static bool						_indexesIsValid(const std::vector<std::string> &indexes);
		// static bool						_defaultErrorPageIsValid(const std::string &default_error_page);
		// static bool						_clientMaxBodySizeIsValid(const int client_max_body_size);
		// static bool						_locationsIsValid(const std::vector<Location> &locations);
	//Custom will be added

	public:
		VirtualServer(const Config::map &config_map) throw(std::exception);
		~VirtualServer(void);
	
	public:
		const std::string				&getDefaultErrorPage(void) const;
		int								getClientMaxBodySize(void) const;
		const std::string				&getRoot(void) const;
		const std::vector<std::string>	&getIndexes(void) const;
		const std::vector<Location>		&getLocations(void) const;
	
	public:
		class InvalidRootException: public std::exception {
			public:
				virtual const char *what() const throw();
		};
		class InvalidIndexesException: public std::exception {
			public:
				virtual const char *what() const throw();
		};
		class InvalidDefaultErrorPageException: public std::exception {
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
		class InvalidLocationException: public std::exception {
			public:
				virtual const char *what() const throw();
		};
		class FailToRunException: public std::exception {
			public:
				virtual const char *what() const throw();
		};
};

std::ostream	&operator<<(std::ostream &os, const VirtualServer &rhs);

#endif