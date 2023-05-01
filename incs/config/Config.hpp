#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <fstream>
# include <string>
# include <vector>
# include <map>

class Server;

class Config {
	public:
		typedef std::map< std::string, std::vector<std::string> >	map;

		const static std::string	LISTEN;
		const static std::string	PORT;
		const static std::string	SERVER_NAME;
		const static std::string	ROOT;
		const static std::string	INDEX;
		const static std::string	ERROR_PAGE;
		const static std::string	CLIENT_MAX_BODY_SIZE;
		const static std::string	CGI_PASS;

	private:
		std::string	_file_name;

		// TODO: Validate file name (extension)

		map		getConfigMap(std::ifstream &infile) const throw(std::exception);
		bool	initConfigMap(map &config_map) const;

	public:
		Config(const char *file_name);
		~Config(void);

		// Util
		bool	initServers(std::vector<Server> &servers) const throw(std::exception);

		// Exception
		class FailToOpenFileException: public std::exception {
			public:
				virtual const char* what() const throw();
		};

		class InvalidSyntaxException: public std::exception {
			public:
				virtual const char* what() const throw();
		};

		class NotEnoughArgumentsException: public std::exception {
			public:
				virtual const char* what() const throw();
		};

		class UnknownException: public std::exception {
			public:
				virtual const char* what() const throw();
		};
};

#endif