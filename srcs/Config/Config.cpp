#include "../../incs/Config/Config.hpp"
#include "../../libs/shared_ptr/shared_ptr.hpp"
#include <iostream>

//Refactoring::hyunkyle Enum

static const std::string	g_keys[] = {
	"listen",
	"server_name",
	"root",
	"index",
	"error_page",
	"client_max_body_size",
	"location"
};
static const std::string	g_location_keys[] = {
	"dir",
	"root",
	"alias",
	"autoindex",
	"return"
};

// Const static variables
const std::vector<std::string>	Config::KEYS(g_keys, g_keys + sizeof(g_keys) / sizeof(std::string));
const std::vector<std::string>	Config::LOCATION_KEYS(g_location_keys, g_location_keys + sizeof(g_location_keys) / sizeof(std::string));
bool	(*const	Config::HANDLERS[])(Config::map &config_map, std::ifstream &infile) = {
	Config::handleListen,
	Config::handleServerName,
	Config::handleRoot,
	Config::handleIndex,
	Config::handleErrorPage,
	Config::handleClientMaxBodySize,
	Config::handleLocation
};
bool	(*const	Config::LOCATION_HANDLERS[])(Config::map &config_map, std::ifstream &infile) = {
	Config::handleLocationDir,
	Config::handleLocationRoot,
	Config::handleLocationAlias,
	Config::handleLocationAutoIndex,
	Config::handleLocationReturn
};

//Refactoring::Hyunkyle-Constructor Fix:method
Config::Config(void): _file_name(""), _config_maps(std::vector<map>()) {}
Config::Config(const char *file_name): _file_name(file_name), _config_maps(std::vector<map>()) {
}

Config::Config(const Config &ref): _file_name(ref._file_name), _config_maps(ref._config_maps) {}
Config::~Config(void) {}
Config	&Config::operator=(const Config &rhs) {
	if (this != &rhs) {
		this->~Config();
		new (this) Config(rhs);
	}
	return (*this);
}

void Config::startParse(void) throw(std::exception) {
	std::ifstream	infile(this->_file_name);
	std::string		token;

	if (Config::invalidFileName(this->_file_name)) {
		throw (InvalidFileNameException());
	}

	if (infile.is_open() == false) {
		throw (FailToOpenFileException());
	}

	while (infile >> token) {
		if (token != "server") {
			throw (InvalidSyntaxException());
		}

		try {
			this->_config_maps.push_back(this->getConfigMapOfEachServer(infile));
		} catch (const std::exception &e) {
			std::cerr << "\033[31m" << "Error: " << e.what() << "\033[0m" << '\n';
			throw (FailToConfigurateException());
		}
	}
}

// Static functions
bool		Config::invalidFileName(const std::string &file_name) {
	if (file_name.length() <= 5) {
		return (true);
	}

	if (file_name.substr(file_name.length() - 5, 5) != ".conf") {
		return (true);
	}

	return (false);
}
bool		Config::initConfigMap(map &config_map) {
	config_map["listen"] = std::vector<std::string>();
	for (size_t i=1; i<KEYS.size(); i++) {
		config_map[KEYS[i]] = std::vector<std::string>();
	}
	config_map["location_dir"] = std::vector<std::string>();
	for (size_t i=0; i<LOCATION_KEYS.size(); i++) {
		config_map[std::string("location_") + LOCATION_KEYS[i]] = std::vector<std::string>();
	}
	return (true);
}
Config::map	Config::getConfigMapOfEachServer(std::ifstream &infile) throw(std::exception) {
	Config::map									ret;
	std::string									token;
	std::vector<std::string>::const_iterator	iter;

	if (!(infile >> token) || token != "{" || initConfigMap(ret) == false) {
		throw (InvalidSyntaxException());
	}

	while (infile >> token) {
		if (token == "}") {
			break ;
		}

		if ((iter = std::find(KEYS.begin(), KEYS.end(), token)) == KEYS.end()) {
			throw (InvalidArgumentException());
		}

		try {
			HANDLERS[iter - KEYS.begin()](ret, infile);
		} catch (const std::exception &e) {
			std::cerr << "\033[31m" << "Error: " << e.what() << "\033[0m" << '\n';
			throw (FailToConfigurateServerException());
		}
	}

	for (size_t i=0; i<KEYS.size()-1; i++) {
		if (ret[KEYS[i]].size() == 0) {
			throw (InvalidSyntaxException());
		}
	}

	return (ret);
}

bool	Config::handleListen(map &config_map, std::ifstream &infile) throw(std::exception) {
	std::string	token;

	if (!(infile >> token) || token.back() != ';') {
		throw (InvalidSyntaxException());
	}

	config_map["listen"].push_back(token.substr(0, token.length() - 1));
	return (true);
}
bool	Config::handleServerName(map &config_map, std::ifstream &infile) throw(std::exception) {
	std::string	token;

	while (infile >> token) {
		if (token.back() == ';') {
			config_map["server_name"].push_back(token.substr(0, token.length() - 1));
			break;
		} else {
			config_map["server_name"].push_back(token);
		}
	}

	if (config_map["server_name"].size() == 0) {
		throw (InvalidSyntaxException());
	}

	return (true);
}
bool	Config::handleRoot(map &config_map, std::ifstream &infile) throw(std::exception) {
	std::string	token;

	if (!(infile >> token) || token.back() != ';') {
		throw (InvalidSyntaxException());
	}

	config_map["root"].push_back(token.substr(0, token.length() - 1));
	return (true);
}

bool	Config::handleIndex(map &config_map, std::ifstream &infile) throw(std::exception) {
	std::string	token;

	while (infile >> token) {
		if (token.back() == ';') {
			config_map["index"].push_back(token.substr(0, token.length() - 1));
			break;
		} else {
			config_map["index"].push_back(token);
		}
	}

	if (config_map["index"].size() == 0) {
		throw (InvalidSyntaxException());
	}

	return (true);
}
bool	Config::handleErrorPage(map &config_map, std::ifstream &infile) throw(std::exception) {
	std::string	token;

	if (!(infile >> token) || token.back() != ';') {
		throw (InvalidSyntaxException());
	}

	config_map["error_page"].push_back(token.substr(0, token.length() - 1));
	return (true);
}
bool	Config::handleClientMaxBodySize(map &config_map, std::ifstream &infile) throw(std::exception) {
	std::string	token;

	if (!(infile >> token) || token.back() != ';') {
		throw (InvalidSyntaxException());
	}

	config_map["client_max_body_size"].push_back(token.substr(0, token.length() - 1));
	return (true);
}
bool	Config::handleLocation(map &config_map, std::ifstream &infile) throw(std::exception) {
	std::string									token;
	std::vector<std::string>::const_iterator	iter;
	bool										initSuccess = false;

	initLocationVector(config_map);

	try {
		LOCATION_HANDLERS[0](config_map, infile);
	} catch (const std::exception &e) {
		std::cerr << "\033[31m" << "Error: " << e.what() << "\033[0m" << '\n';
		throw (FailToConfigurateServerException());
	}

	if (!(infile >> token) || token != "{") {
		throw (InvalidSyntaxException());
	}

	while (infile >> token) {
		if (token == "}") {
			break;
		}

		if ((iter = std::find(LOCATION_KEYS.begin(), LOCATION_KEYS.end(), token)) == LOCATION_KEYS.end()) {
			throw (InvalidArgumentException());
		}

		try {
			LOCATION_HANDLERS[iter - LOCATION_KEYS.begin()](config_map, infile);
		} catch (const std::exception &e) {
			std::cerr << "\033[31m" << "Error: " << e.what() << "\033[0m" << '\n';
			throw (FailToConfigurateLocationException());
		}
	}

	for (size_t i=0; i<LOCATION_KEYS.size(); i++) {
		if (config_map[std::string("location_") + LOCATION_KEYS[i]].back() != "") {
			initSuccess = true;
		}
	}

	if (!initSuccess) {
		throw (NotEnoughArgumentsException());
	}

	return (true);
}

bool	Config::initLocationVector(map &config_map) {
	for (size_t i=0; i<LOCATION_KEYS.size(); i++) {
		config_map[std::string("location_") + LOCATION_KEYS[i]].push_back("");
	}

	return (true);
}
bool	Config::handleLocationDir(map &config_map, std::ifstream &infile) throw(std::exception) {
	std::string	token;

	if (!(infile >> token)) {
		throw (InvalidSyntaxException());
	}

	config_map["location_dir"].back() = token;
	return (true);
}
bool	Config::handleLocationRoot(map &config_map, std::ifstream &infile) throw(std::exception) {
	std::string	token;

	if (!(infile >> token) || token.back() != ';') {
		throw (InvalidSyntaxException());
	}

	config_map["location_root"].back() = token.substr(0, token.length() - 1);
	return (true);
}
bool	Config::handleLocationAlias(map &config_map, std::ifstream &infile) throw(std::exception) {
	std::string	token;

	if (!(infile >> token) || token.back() != ';') {
		throw (InvalidSyntaxException());
	}

	config_map["location_alias"].back() = token.substr(0, token.length() - 1);
	return (true);
}
bool	Config::handleLocationAutoIndex(map &config_map, std::ifstream &infile) throw(std::exception) {
	std::string	token;

	if (!(infile >> token) || token.back() != ';') {
		throw (InvalidSyntaxException());
	}

	config_map["location_autoindex"].back() = token.substr(0, token.length() - 1);
	return (true);
}
bool	Config::handleLocationReturn(map &config_map, std::ifstream &infile) throw(std::exception) {
	std::string	token;

	if (!(infile >> token) || !(infile >> token) || token.back() != ';') {
		throw (InvalidSyntaxException());
	}

	config_map["location_return"].back() = token.substr(0, token.length() - 1);
	return (true);
}

// Getter
const std::vector<Config::map>	&Config::getConfigMaps(void) const { return (this->_config_maps); }

// Exception
const char	*Config::InvalidFileNameException::what() const throw() { return ("Config: Invalid file name"); }
const char	*Config::FailToOpenFileException::what() const throw() { return ("Config: Fail to open file"); }
const char	*Config::InvalidSyntaxException::what() const throw() { return ("Config: Invalid syntax"); }
const char	*Config::InvalidArgumentException::what() const throw() { return ("Config: Invalid argument"); }
const char	*Config::NotEnoughArgumentsException::what() const throw() { return ("Config: Not enough arguments"); }
const char	*Config::FailToConfigurateLocationException::what() const throw() { return ("Config: Fail to configurate location"); }
const char	*Config::FailToConfigurateServerException::what() const throw() { return ("Config: Fail to configurate server"); }
const char	*Config::FailToConfigurateException::what() const throw() { return ("Config: Fail to configurate"); }