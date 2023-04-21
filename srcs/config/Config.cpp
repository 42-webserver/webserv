#include "../../incs/config/Config.hpp"

Config::Config(const char *file_name): _file_name(file_name) {}
Config::~Config(void) {}

// Util
bool	Config::setServer(Server &server, std::ifstream &infile) const throw(std::exception){
	std::string	token;

	infile >> token;
	if (token != "{") {
		throw (Config::InvalidSyntaxException());
	}

	while (infile >> token) {
		if (token == "listen") {
			infile >> token;
			if (token == "80;") {
				server.setPort(80);
			} else if (token == "443;") {
				server.setPort(443);
			} else {
				throw (Config::InvalidSyntaxException());
			}
		} else if (token == "server_name") {
			while (infile >> token && token.back() != ';') {
				server.pushName(token);
			}
			server.pushName(token.substr(0, token.size()-1));
		} else if (token == "client_max_body_size") {
			infile >> token;
			server.setClientMaxBodySize(std::stoi(token.substr(0, token.size()-1)));
			//TODO: calculate unit size
		} else if (token == "root") {
			infile >> token;
			server.setRoot(token.substr(0, token.size()-1));
		} else if (token == "index") {
			while (infile >> token && token.back() != ';') {
				server.pushIndex(token);
			}
			server.pushIndex(token.substr(0, token.size()-1));
		} else if (token == "cgi_pass") {
			infile >> token;
			server.setCgiPass(token.substr(0, token.size()-1));
		} else if (token == "}") {
			break ;
		} else {
			throw (Config::UnknownException());
		}
	}
	return (true);
}

bool	Config::setServers(std::vector<Server> &servers) const throw(std::exception) {
	std::ifstream	infile(this->_file_name);
	std::string		token;

	if (infile.is_open() == false) {
		throw (Config::FailToOpenFileException());
	}

	while (infile >> token) {
		if (token != "server") {
			throw (Config::InvalidSyntaxException());
		}

		Server	server;

		try {
			this->setServer(server, infile);
		} catch (Config::InvalidSyntaxException &e) {
			throw (e);
		} catch (...) {
			throw (Config::UnknownException());
		}
		servers.push_back(server);
	}
	return (true);
}

// Exception
const char	*Config::FailToOpenFileException::what() const throw() { return ("Fail to open file"); }
const char	*Config::InvalidSyntaxException::what() const throw() { return ("Invalid syntax"); }
const char	*Config::UnknownException::what() const throw() { return ("Unknown Exception"); }