#include "../../incs/server/Server.hpp"
#include "../../incs/socket/PassiveSocket.hpp"
#include <unistd.h>
#include <iostream>
#include <errno.h>

Server::Server(const Config::map &config_map) throw(std::exception):
	Data(),
	_port(std::atoi(config_map.find(Config::PORT)->second[0].c_str())),
	_names(config_map.find(Config::SERVER_NAME)->second),
	_root(config_map.find(Config::ROOT)->second[0]),
	_indexes(config_map.find(Config::INDEX)->second),
	_default_error_page(config_map.find(Config::ERROR_PAGE)->second[0]),
	_client_max_body_size(std::atoi(config_map.find(Config::CLIENT_MAX_BODY_SIZE)->second[0].c_str())),
	_cgi_pass(config_map.find(Config::CGI_PASS)->second[0])
{
	if (Server::_is_valid_port(_port) == false) {
		throw (InvalidPortException());
	}
	if (Server::_is_valid_client_max_body_size(_client_max_body_size) == false) {
		throw (InvalidClientMaxBodySizeException());
	}

	try {
		this->_socket = ft::shared_ptr<Socket>(new PassiveSocket(this->_port));
	} catch (const std::exception &e) {
		std::cerr << "\033[31m" << "Error: " << e.what() << "\033[0m" << '\n';
		throw (FailToCreateSocketException());
	}
}
Server::~Server(void) {}

// Static
bool	Server::_is_valid_port(int port) { return (0 <= port && port <= 65535); }
bool	Server::_is_valid_client_max_body_size(int client_max_body_size) { return (0 <= client_max_body_size); }

// Util
int		Server::accept(void) const {
	int	client_fd = -1;

	try {
		client_fd = dynamic_cast<PassiveSocket *>(this->_socket.get())->accept();
	} catch (const std::exception &e) {
		std::cerr << "\033[31m" << "Error: " << e.what() << "\033[0m" << '\n';
	}
	return (client_fd);
}

// Getters
const std::string				&Server::getDefaultErrorPage(void) const { return (this->_default_error_page); }
int								Server::getPort(void) const { return (this->_port); }
const std::vector<std::string>	&Server::getNames(void) const { return (this->_names); }
int								Server::getClientMaxBodySize(void) const { return (this->_client_max_body_size); }
const std::string				&Server::getRoot(void) const { return (this->_root); }
const std::vector<std::string>	&Server::getIndexes(void) const { return (this->_indexes); }
const std::string				&Server::getCgiPass(void) const { return (this->_cgi_pass); }

// Operator overload
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

// Exception
const char	*Server::InvalidPortException::what(void) const throw() { return ("Server: Invalid port"); }
const char	*Server::InvalidClientMaxBodySizeException::what(void) const throw() { return ("Server: Invalid client_max_body_size"); }
const char	*Server::FailToCreateSocketException::what(void) const throw() { return ("Server: Fail to create socket"); }
const char	*Server::FailToRunException::what(void) const throw() { return ("Server: Fail to run"); }