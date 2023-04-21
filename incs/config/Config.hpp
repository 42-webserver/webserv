#ifndef CONFIG_HPP
# define CONFIG_HPP

#include "../incs/server/Server.hpp"
#include <fstream>
#include <string>
#include <vector>

class Config {
	private:
		std::string	_file_name;

		bool	setServer(Server &server, std::ifstream &infile) const throw(std::exception);

	public:
		Config(const char *file_name);
		~Config(void);

		// Util
		bool	setServers(std::vector<Server> &servers) const throw(std::exception);

		// Exception
		class FailToOpenFileException: public std::exception {
			public:
				virtual const char* what() const throw();
		};

		class InvalidSyntaxException: public std::exception {
			public:
				virtual const char* what() const throw();
		};

		class UnknownException: public std::exception {
			public:
				virtual const char* what() const throw();
		};
};

#endif