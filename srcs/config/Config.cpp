#include "../../incs/config/Config.hpp"
#include "../../incs/server/Server.hpp"
#include "../../libs/shared_ptr/incs/shared_ptr.hpp"

// Static
const std::string	Config::LISTEN = "listen";
const std::string	Config::PORT = "port";
const std::string	Config::SERVER_NAME = "server_name";
const std::string	Config::ROOT = "root";
const std::string	Config::INDEX = "index";
const std::string	Config::ERROR_PAGE = "error_page";
const std::string	Config::CLIENT_MAX_BODY_SIZE = "client_max_body_size";
const std::string	Config::CGI_PASS = "cgi_pass";

Config::Config(void): _file_name(""), _config_maps(std::vector< map >()) {}
Config::Config(const char *file_name): _file_name(file_name), _config_maps(std::vector< map >()) {
	std::ifstream	infile(this->_file_name);
	std::string		token;

	if (Config::invalidFileName(file_name)) {
		throw (Config::InvalidFileNameException());
	}

	if (infile.is_open() == false) {
		throw (Config::FailToOpenFileException());
	}

	while (infile >> token) {
		if (token != "server") {
			throw (Config::InvalidSyntaxException());
		}

		try {
			this->_config_maps.push_back(this->getConfigMapOfEachServer(infile));
		} catch (const Config::InvalidSyntaxException &e) {
			throw (e);
		} catch (const Config::NotEnoughArgumentsException &e) {
			throw (e);
		}
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

bool							Config::invalidFileName(const std::string &file_name) {
	if (file_name.length() <= 5) {
		return (true);
	}

	if (file_name.substr(file_name.length() - 5, 5) != ".conf") {
		return (true);
	}

	return (false);
}
Config::map						Config::getConfigMapOfEachServer(std::ifstream &infile) const throw(std::exception) {
	Config::map	ret;
	std::string	token;

	if (!(infile >> token) || token != "{" || initConfigMap(ret) == false) {
		throw (Config::InvalidSyntaxException());
	}

	while (infile >> token) {
		if (token == "}") {
			break ;
		} else if (token == Config::LISTEN) {
			if (!(infile >> token) || token.back() != ';') {
				throw (Config::InvalidSyntaxException());
			}

			ret[Config::PORT].push_back(token.substr(0, token.length() - 1));
		} else if (token == Config::SERVER_NAME) {
			while (infile >> token) {
				ret[Config::SERVER_NAME].push_back(token.substr(0, token.find(';')));

				if (token.back() == ';') {
					break ;
				}
			}
		} else if (token == Config::ROOT) {
			if (!(infile >> token) || token.back() != ';') {
				throw (Config::InvalidSyntaxException());
			}

			ret[Config::ROOT].push_back(token.substr(0, token.length() - 1));
		} else if (token == Config::INDEX) {
			while (infile >> token) {
				ret[Config::INDEX].push_back(token.substr(0, token.find(';')));

				if (token.back() == ';') {
					break ;
				}
			}
		} else if (token == Config::ERROR_PAGE) {
			if (!(infile >> token) || token.back() != ';') {
				throw (Config::InvalidSyntaxException());
			}

			ret[Config::ERROR_PAGE].push_back(token.substr(0, token.length() - 1));
		} else if (token == Config::CLIENT_MAX_BODY_SIZE) {
			if (!(infile >> token) || token.back() != ';') {
				throw (Config::InvalidSyntaxException());
			}

			ret[Config::CLIENT_MAX_BODY_SIZE].push_back(token.substr(0, token.length() - 1));
		} else if (token == Config::CGI_PASS) {
			if (!(infile >> token) || token.back() != ';') {
				throw (Config::InvalidSyntaxException());
			}

			ret[Config::CGI_PASS].push_back(token.substr(0, token.length() - 1));
		} else {
			throw (Config::InvalidSyntaxException());
		}
	}

	for (map::iterator it = ret.begin(); it != ret.end(); it++) {
		if (it->second.size() == 0) {
			throw (Config::NotEnoughArgumentsException());
		}
	}

	return (ret);
}
bool							Config::initConfigMap(map &config_map) const {
	config_map[Config::PORT] = std::vector<std::string>();
	config_map[Config::SERVER_NAME] = std::vector<std::string>();
	config_map[Config::ROOT] = std::vector<std::string>();
	config_map[Config::INDEX] = std::vector<std::string>();
	config_map[Config::ERROR_PAGE] = std::vector<std::string>();
	config_map[Config::CLIENT_MAX_BODY_SIZE] = std::vector<std::string>();
	config_map[Config::CGI_PASS] = std::vector<std::string>();
	return (true);
}
const std::vector<Config::map>	&Config::getConfigMaps(void) const { return (this->_config_maps); }

// Exception
const char	*Config::InvalidFileNameException::what() const throw() { return ("Config: Invalid file name"); }
const char	*Config::FailToOpenFileException::what() const throw() { return ("Config: Fail to open file"); }
const char	*Config::InvalidSyntaxException::what() const throw() { return ("Config: Invalid syntax"); }
const char	*Config::NotEnoughArgumentsException::what() const throw() { return ("Config: Not enough arguments"); }