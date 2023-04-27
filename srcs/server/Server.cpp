#include "../../incs/server/Server.hpp"
#include "../../incs/config/Config.hpp"

const std::string	Server::_default_err_page = "err.html";

Server::Server(const std::map< std::string, std::vector< std::string > > &config_map):
	_port(std::atoi(config_map.find(Config::PORT)->second[0].c_str())),
	_names(config_map.find(Config::SERVER_NAME)->second),
	_client_max_body_size(std::atoi(config_map.find(Config::CLIENT_MAX_BODY_SIZE)->second[0].c_str())),
	_root(config_map.find(Config::ROOT)->second[0]),
	_indexes(config_map.find(Config::INDEX)->second),
	_cgi_pass(config_map.find(Config::CGI_PASS)->second[0])
{
	if (_port < 0 || _port > 65535)
		throw Server::InvalidPortException();
	if (_client_max_body_size < 0)
		throw Server::InvalidClientMaxBodySizeException();
}
Server::~Server(void) {}

// Getters
const std::string				&Server::getDefaultErrPage(void) { return (_default_err_page); }
int								Server::getPort(void) const { return (_port); }
const std::vector<std::string>	&Server::getNames(void) const { return (_names); }
int								Server::getClientMaxBodySize(void) const { return (_client_max_body_size); }
const std::string				&Server::getRoot(void) const { return (_root); }
const std::vector<std::string>	&Server::getIndexes(void) const { return (_indexes); }
const std::string				&Server::getCgiPass(void) const { return (_cgi_pass); }

// Exception
const char	*Server::InvalidPortException::what(void) const throw() { return ("Invalid port"); }
const char	*Server::InvalidClientMaxBodySizeException::what(void) const throw() { return ("Invalid client_max_body_size"); }

std::ostream	&operator<<(std::ostream &os, const Server &rhs) {
	os << "port: " << rhs.getPort() << '\n';
	os << "names: ";
	for (size_t i=0; i<rhs.getNames().size(); i++) {
		os << rhs.getNames()[i] << ' ';
	}
	os << '\n';
	os << "client_max_body_size: " << rhs.getClientMaxBodySize() << '\n';
	os << "root: " << rhs.getRoot() << '\n';
	os << "indexes: ";
	for (size_t i=0; i<rhs.getIndexes().size(); i++) {
		os << rhs.getIndexes()[i] << ' ';
	}
	os << '\n';
	os << "cgi_pass: " << rhs.getCgiPass();
	return (os);
}