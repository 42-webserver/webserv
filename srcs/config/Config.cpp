#include "../../incs/config/Config.hpp"

// Static
const std::string	Config::LISTEN = "listen";
const std::string	Config::PORT = "port";
const std::string	Config::SERVER_NAME = "server_name";
const std::string	Config::ROOT = "root";
const std::string	Config::INDEX = "index";
const std::string	Config::CLIENT_MAX_BODY_SIZE = "client_max_body_size";
const std::string	Config::CGI_PASS = "cgi_pass";

Config::Config(const char *file_name): _file_name(file_name) {}
Config::~Config(void) {}

// Util
std::map< std::string, std::vector< std::string > >	Config::getConfigMap(std::ifstream &infile) const throw(std::exception) {
	std::map< std::string, std::vector< std::string > >	ret;
	std::string											token;

	if (!(infile >> token) || token != "{") {
		throw (Config::InvalidSyntaxException());
	}

	while (infile >> token) {
		if (token == "}") {
			break ;
		} else if (token == Config::LISTEN) {
			if (!(infile >> token) || token.back() != ';') {
				throw (Config::InvalidSyntaxException());
			}

			ret[Config::PORT] = std::vector<std::string>(1, token.substr(0, token.length() - 1));
		} else if (token == Config::SERVER_NAME) {
			ret[Config::SERVER_NAME] = std::vector<std::string>();

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

			ret[Config::ROOT] = std::vector<std::string>(1, token.substr(0, token.length() - 1));
		} else if (token == Config::INDEX) {
			ret[Config::INDEX] = std::vector<std::string>();

			while (infile >> token) {
				ret[Config::INDEX].push_back(token.substr(0, token.find(';')));

				if (token.back() == ';') {
					break ;
				}
			}
		} else if (token == Config::CLIENT_MAX_BODY_SIZE) {
			if (!(infile >> token) || token.back() != ';') {
				throw (Config::InvalidSyntaxException());
			}

			ret[Config::CLIENT_MAX_BODY_SIZE] = std::vector<std::string>(1, token.substr(0, token.length() - 1));
		} else if (token == Config::CGI_PASS) {
			if (!(infile >> token) || token.back() != ';') {
				throw (Config::InvalidSyntaxException());
			}

			ret[Config::CGI_PASS] = std::vector<std::string>(1, token.substr(0, token.length() - 1));
		} else {
			throw (Config::InvalidSyntaxException());
		}
	}

	for (std::map< std::string, std::vector< std::string > >::iterator it = ret.begin(); it != ret.end(); it++) {
		if (it->second.size() == 0) {
			throw (Config::NotEnoughArgumentsException());
		}
	}

	return (ret);
}

bool	Config::initServers(std::vector<Server> &servers) const throw(std::exception) {
	std::ifstream	infile(this->_file_name);
	std::string		token;

	if (infile.is_open() == false) {
		throw (Config::FailToOpenFileException());
	}

	while (infile >> token) {
		if (token != "server") {
			throw (Config::InvalidSyntaxException());
		}

		try {
			servers.push_back(Server(this->getConfigMap(infile)));
		} catch (const Config::InvalidSyntaxException &e) {
			throw (e);
		} catch (const Server::InvalidPortException &e) {
			throw (e);
		} catch (const Server::InvalidClientMaxBodySizeException &e) {
			throw (e);
		} catch (...) {
			throw (Config::UnknownException());
		}
	}
	return (true);
}

// Exception
const char	*Config::FailToOpenFileException::what() const throw() { return ("Fail to open file"); }
const char	*Config::InvalidSyntaxException::what() const throw() { return ("Invalid syntax"); }
const char	*Config::NotEnoughArgumentsException::what() const throw() { return ("Not enough arguments"); }
const char	*Config::UnknownException::what() const throw() { return ("Unknown Exception"); }