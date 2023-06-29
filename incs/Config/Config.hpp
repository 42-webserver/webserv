#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <fstream>
# include <string>
# include <vector>
# include <map>

class Config {
	public:
		typedef std::map< std::string, std::vector<std::string> >	map;

	public:
		struct KEY {
			enum e_key {
				LISTEN,
				SERVER_NAME,
				ROOT,
				INDEX,
				ERROR_PAGE,
				CLIENT_MAX_BODY_SIZE,
				LOCATION
			};
		};
		struct LOCATION_KEY {
			enum e_location_key {
				DIR,
				ROOT,
				ALIAS,
				AUTO_INDEX,
				RETURN_STATUS,
				RETURN_URL
			};
		};

	public:
		const static std::vector<std::string>	KEYS;
		const static std::vector<std::string>	LOCATION_KEYS;
	
	private:
		static bool	(*const	HANDLERS[])(map &config_map, std::ifstream &infile);
		static bool	(*const	LOCATION_HANDLERS[])(map &config_map, std::ifstream &infile);

	private:
		std::string			_file_name;
		std::vector<map>	_config_maps;
	
	private:
		bool	_init(void) throw(std::exception);

	private:
		static bool	invalidFileName(const std::string &file_name);
		static bool	initConfigMap(map &config_map);
		static map	getConfigMapOfEachServer(std::ifstream &infile) throw(std::exception);

	private:
		static bool	handleListen(map &config_map, std::ifstream &infile) throw(std::exception);
		static bool	handleServerName(map &config_map, std::ifstream &infile) throw(std::exception);
		static bool	handleRoot(map &config_map, std::ifstream &infile) throw(std::exception);
		static bool	handleIndex(map &config_map, std::ifstream &infile) throw(std::exception);
		static bool	handleErrorPage(map &config_map, std::ifstream &infile) throw(std::exception);
		static bool	handleClientMaxBodySize(map &config_map, std::ifstream &infile) throw(std::exception);
		static bool	handleLocation(map &config_map, std::ifstream &infile) throw(std::exception);
	
	private:
		static bool	handleLocationDir(map &config_map, std::ifstream &infile) throw(std::exception);
		static bool	handleLocationRoot(map &config_map, std::ifstream &infile) throw(std::exception);
		static bool	handleLocationAlias(map &config_map, std::ifstream &infile) throw(std::exception);
		static bool	handleLocationAutoIndex(map &config_map, std::ifstream &infile) throw(std::exception);
		static bool	handleLocationReturnStatus(map &config_map, std::ifstream &infile) throw(std::exception);
		static bool	handleLocationReturnUrl(map &config_map, std::ifstream &infile) throw(std::exception);

	public:
		Config(void);
		Config(const char *file_name);
		Config(const Config &ref);
		~Config(void);
		Config	&operator=(const Config &rhs);

	public:
		const std::vector<map>	&getConfigMaps(void) const;

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
		class InvalidArgumentException: public std::exception {
			public:
				virtual const char* what() const throw();
		};
		class NotEnoughArgumentsException: public std::exception {
			public:
				virtual const char* what() const throw();
		};
		class FailToConfigurateLocationException: public std::exception {
			public:
				virtual const char* what() const throw();
		};
		class FailToConfigurateServerException: public std::exception {
			public:
				virtual const char* what() const throw();
		};
		class FailToConfigurateException: public std::exception {
			public:
				virtual const char* what() const throw();
		};
		class FailToConstructException: public std::exception {
			public:
				virtual const char* what() const throw();
		};
};

#endif