#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <fstream>
# include <string>
# include <vector>
# include <map>

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
		static bool	invalidFileName(const std::string &file_name);

	private:
		std::string			_file_name;
		std::vector< map >	_config_maps;

		bool	initConfigMap(map &config_map) const;
		map		getConfigMapOfEachServer(std::ifstream &infile) const throw(std::exception);

	public:
		Config(void);
		Config(const char *file_name);
		Config(const Config &ref);
		~Config(void);
		Config	&operator=(const Config &rhs);

	public:
		const std::vector< map >	&getConfigMaps(void) const;

	public:
		class InvalidFileNameException: public std::exception {
			public:
				virtual const char* what() const throw();
		};
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
};

#endif