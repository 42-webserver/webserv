#ifndef LOCATION_HPP
# define LOCATION_HPP

# include "../config/Config.hpp"
# include <string>
# include <map>
# include <vector>
# include <iostream>

class Location {
	private:
		const std::string	_dir;
		const std::string	_root;
		const std::string	_alias;
		const std::string	_autoindex;
		const std::string	_return_status;
		const std::string	_return_url;
	
	private:
		std::string	parseDir(const Config::map &config_map, size_t idx) throw(std::exception);
		std::string	parseRoot(const Config::map &config_map, size_t idx) throw(std::exception);
		std::string	parseAlias(const Config::map &config_map, size_t idx) throw(std::exception);
		std::string	parseAutoIndex(const Config::map &config_map, size_t idx) throw(std::exception);
		std::string	parseReturnStatus(const Config::map &config_map, size_t idx) throw(std::exception);
		std::string	parseReturnUrl(const Config::map &config_map, size_t idx) throw(std::exception);

	public:
		Location(void);
		Location(const Config::map &config_map, size_t idx);
		Location(const Location &ref);
		~Location(void);
		Location &operator=(const Location &rhs);

	public:
		const std::string	&getDir(void) const;
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
		class InvalidAutoIndexValueException: public std::exception {
			public:
				virtual const char *what() const throw();
		};
};

std::ostream &operator<<(std::ostream &os, const Location &location);

#endif