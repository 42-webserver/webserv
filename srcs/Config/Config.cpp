#include "../../libs/shared_ptr/shared_ptr.hpp"
#include "../../incs/Config/Config.hpp"
#include "../../incs/Log/Logger.hpp"
#include <iostream>

const static std::string	g_keys[] = {
	"listen",
	"server_name",
	"root",
	"index",
	"error_page",
	"client_max_body_size",
	"location"
};
const static std::string	g_location_keys[] = {
	"dir",
	"root",
	"alias",
	"autoindex",
	"return_status",
	"return_url"
};

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
	Config::handleLocationReturnStatus,
	Config::handleLocationReturnUrl
};

Config::Config(void): _file_name(""), _config_maps(std::vector<map>()) {}
Config::Config(const char *file_name): _file_name(file_name), _config_maps(std::vector<map>()) {
	try {
		this->_init();
	} catch (const std::exception &e) {
		Logger::getInstance().error(e.what());
		throw (FailToConstructException());
	}
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

bool							Config::_init(void) throw(std::exception) {
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
			Logger::getInstance().error(e.what());
			throw (FailToConfigurateException());
		}
	}

	return (true);
}
const std::vector<Config::map>	&Config::getConfigMaps(void) const { return (this->_config_maps); }

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
	config_map[Config::KEYS[Config::KEY::LISTEN]] = std::vector<std::string>();
	for (size_t i=1; i<KEYS.size(); i++) {
		config_map[KEYS[i]] = std::vector<std::string>();
	}
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
			Logger::getInstance().error(e.what());
			throw (FailToConfigurateServerException());
		}
	}

	for (size_t i=0; i<KEYS.size()-1; i++) {
		if (ret[KEYS[i]].size() == 0) {
			throw (NotEnoughArgumentsException());
		}
	}

	return (ret);
}

bool	Config::handleListen(map &config_map, std::ifstream &infile) throw(std::exception) {
	std::string	token;

	if (!(infile >> token) || token.back() != ';') {
		throw (InvalidSyntaxException());
	}

	config_map[Config::KEYS[Config::KEY::LISTEN]].push_back(token.substr(0, token.length() - 1));
	return (true);
}
bool	Config::handleServerName(map &config_map, std::ifstream &infile) throw(std::exception) {
	std::string	token;

	while (infile >> token) {
		if (token.back() == ';') {
			config_map[Config::KEYS[Config::KEY::SERVER_NAME]].push_back(token.substr(0, token.length() - 1));
			break;
		} else {
			config_map[Config::KEYS[Config::KEY::SERVER_NAME]].push_back(token);
		}
	}

	if (config_map[Config::KEYS[Config::KEY::SERVER_NAME]].size() == 0) {
		throw (InvalidSyntaxException());
	}

	return (true);
}
bool	Config::handleRoot(map &config_map, std::ifstream &infile) throw(std::exception) {
	std::string	token;

	if (!(infile >> token) || token.back() != ';') {
		throw (InvalidSyntaxException());
	}

	config_map[Config::KEYS[Config::KEY::ROOT]].push_back(token.substr(0, token.length() - 1));
	return (true);
}
bool	Config::handleIndex(map &config_map, std::ifstream &infile) throw(std::exception) {
	std::string	token;

	while (infile >> token) {
		if (token.back() == ';') {
			config_map[Config::KEYS[Config::KEY::INDEX]].push_back(token.substr(0, token.length() - 1));
			break;
		} else {
			config_map[Config::KEYS[Config::KEY::INDEX]].push_back(token);
		}
	}

	if (config_map[Config::KEYS[Config::KEY::INDEX]].size() == 0) {
		throw (InvalidSyntaxException());
	}

	return (true);
}
bool	Config::handleErrorPage(map &config_map, std::ifstream &infile) throw(std::exception) {
	std::string	token;

	if (!(infile >> token) || token.back() != ';') {
		throw (InvalidSyntaxException());
	}

	config_map[Config::KEYS[Config::KEY::ERROR_PAGE]].push_back(token.substr(0, token.length() - 1));
	return (true);
}
bool	Config::handleClientMaxBodySize(map &config_map, std::ifstream &infile) throw(std::exception) {
	std::string	token;

	if (!(infile >> token) || token.back() != ';') {
		throw (InvalidSyntaxException());
	}

	config_map[Config::KEYS[Config::KEY::CLIENT_MAX_BODY_SIZE]].push_back(token.substr(0, token.length() - 1));
	return (true);
}
bool	Config::handleLocation(map &config_map, std::ifstream &infile) throw(std::exception) {
	std::string									token;
	std::vector<std::string>::const_iterator	iter;

	try {
		LOCATION_HANDLERS[0](config_map, infile);
	} catch (const std::exception &e) {
		Logger::getInstance().error(e.what());
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
			Logger::getInstance().error(e.what());
			throw (FailToConfigurateLocationException());
		}
	}

	for (size_t i=0; i<LOCATION_KEYS.size(); i++) {
		if (0 < config_map[std::string("location_") + LOCATION_KEYS[i]].size()) {
			return (true);
		}
	}

	throw (InvalidSyntaxException());
}

bool	Config::handleLocationDir(map &config_map, std::ifstream &infile) throw(std::exception) {
	std::string	token;

	if (!(infile >> token)) {
		throw (InvalidSyntaxException());
	}

	config_map[std::string("location_") + Config::LOCATION_KEYS[Config::LOCATION_KEY::DIR]].push_back(token);
	config_map[std::string("location_") + Config::LOCATION_KEYS[Config::LOCATION_KEY::ROOT]].push_back("");
	config_map[std::string("location_") + Config::LOCATION_KEYS[Config::LOCATION_KEY::ALIAS]].push_back("");
	config_map[std::string("location_") + Config::LOCATION_KEYS[Config::LOCATION_KEY::AUTO_INDEX]].push_back("");
	config_map[std::string("location_") + Config::LOCATION_KEYS[Config::LOCATION_KEY::RETURN_STATUS]].push_back("");
	config_map[std::string("location_") + Config::LOCATION_KEYS[Config::LOCATION_KEY::RETURN_URL]].push_back("");
	return (true);
}
bool	Config::handleLocationRoot(map &config_map, std::ifstream &infile) throw(std::exception) {
	std::string	token;

	if (!(infile >> token) || token.back() != ';') {
		throw (InvalidSyntaxException());
	}

	config_map[std::string("location_") + Config::LOCATION_KEYS[Config::LOCATION_KEY::ROOT]].back() = token.substr(0, token.length() - 1);
	return (true);
}
bool	Config::handleLocationAlias(map &config_map, std::ifstream &infile) throw(std::exception) {
	std::string	token;

	if (!(infile >> token) || token.back() != ';') {
		throw (InvalidSyntaxException());
	}

	config_map[std::string("location_") + Config::LOCATION_KEYS[Config::LOCATION_KEY::ALIAS]].back() = token.substr(0, token.length() - 1);
	return (true);
}
bool	Config::handleLocationAutoIndex(map &config_map, std::ifstream &infile) throw(std::exception) {
	std::string	token;

	if (!(infile >> token) || token.back() != ';') {
		throw (InvalidSyntaxException());
	}

	config_map[std::string("location_") + Config::LOCATION_KEYS[Config::LOCATION_KEY::AUTO_INDEX]].back() = token.substr(0, token.length() - 1);
	return (true);
}
bool	Config::handleLocationReturnStatus(map &config_map, std::ifstream &infile) throw(std::exception) {
	std::string	token;

	if (!(infile >> token) || token.back() != ';') {
		throw (InvalidSyntaxException());
	}

	config_map[std::string("location_") + Config::LOCATION_KEYS[Config::LOCATION_KEY::RETURN_STATUS]].back() = token.substr(0, token.length() - 1);
	return (true);
}
bool	Config::handleLocationReturnUrl(map &config_map, std::ifstream &infile) throw(std::exception) {
	std::string	token;

	if (!(infile >> token) || token.back() != ';') {
		throw (InvalidSyntaxException());
	}

	config_map[std::string("location_") + Config::LOCATION_KEYS[Config::LOCATION_KEY::RETURN_URL]].back() = token.substr(0, token.length() - 1);
	return (true);
}

// Exception
const char	*Config::InvalidFileNameException::what() const throw() { return ("Config: Invalid file name"); }
const char	*Config::FailToOpenFileException::what() const throw() { return ("Config: Fail to open file"); }
const char	*Config::InvalidSyntaxException::what() const throw() { return ("Config: Invalid syntax"); }
const char	*Config::InvalidArgumentException::what() const throw() { return ("Config: Invalid argument"); }
const char	*Config::NotEnoughArgumentsException::what() const throw() { return ("Config: Not enough arguments"); }
const char	*Config::FailToConfigurateLocationException::what() const throw() { return ("Config: Fail to configurate location"); }
const char	*Config::FailToConfigurateServerException::what() const throw() { return ("Config: Fail to configurate server"); }
const char	*Config::FailToConfigurateException::what() const throw() { return ("Config: Fail to configurate"); }
const char	*Config::FailToConstructException::what() const throw() { return ("Config: Fail to construct"); }