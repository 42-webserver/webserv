#ifndef CONFIG_HPP
# define CONFIG_HPP

#include "../incs/Server/Server.hpp"
#include <string>
#include <vector>

class Config {
	private:
		std::string	_file_name;

		// bool	setServer(Server &server, File_pointer &fp) const;

	public:
		Config(const char *file_name);
		~Config(void);

		// Util
		bool	setServers(std::vector<Server> &servers) const;

		// Exception
		class ParsingException: public std::exception {
			public:
				virtual const char* what() const throw();
		};

		class UnknownException: public std::exception {
			public:
				virtual const char* what() const throw();
		};
};

#endif