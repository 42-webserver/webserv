#ifndef LOCATION_HPP
# define LOCATION_HPP

# include "../Config/Config.hpp"
# include <iostream>
# include <string>
# include <map>

class Location {
	private:
		const std::string	_root;
		const std::string	_alias;
		const std::string	_autoindex;
		const std::string	_return_status;
		const std::string	_return_url;
	
	private:
		static std::string	_parseRoot(const Config::map &config_map, size_t idx) throw(std::exception);
		static std::string	_parseAlias(const Config::map &config_map, size_t idx) throw(std::exception);
		static std::string	_parseAutoIndex(const Config::map &config_map, size_t idx) throw(std::exception);
		static std::string	_parseReturnStatus(const Config::map &config_map, size_t idx) throw(std::exception);
		static std::string	_parseReturnUrl(const Config::map &config_map, size_t idx) throw(std::exception);
		static bool			_rootIsValid(const std::string &root);
		static bool			_aliasIsValid(const std::string &alias);
		static bool			_autoIndexIsValid(const std::string &autoindex);
		static bool			_returnStatusIsValid(const std::string &return_status);
		static bool			_returnUrlIsValid(const std::string &return_url);

	public:
		Location(void);
		Location(const Config::map &config_map, size_t idx);
		Location(const Location &ref);
		~Location(void);
		Location &operator=(const Location &rhs);

	public:
		const std::string	&getRoot(void) const;
		const std::string	&getAlias(void) const;
		const std::string	&getAutoIndex(void) const;
		const std::string	&getReturnStatus(void) const;
		const std::string	&getReturnUrl(void) const;
	
	public:
		class InvalidSyntaxException: public std::exception {
			public:
				virtual const char *what() const throw();
		};
		class InvalidRootException: public std::exception {
			public:
				virtual const char *what() const throw();
		};
		class InvalidAliasException: public std::exception {
			public:
				virtual const char *what() const throw();
		};
		class InvalidAutoIndexException: public std::exception {
			public:
				virtual const char *what() const throw();
		};
		class InvalidReturnStatusException: public std::exception {
			public:
				virtual const char *what() const throw();
		};
		class InvalidReturnUrlException: public std::exception {
			public:
				virtual const char *what() const throw();
		};

		friend std::ostream &operator<<(std::ostream &os, const Location &location);
};

#endif