#include "../../incs/server/Server.hpp"
#include <unistd.h>
#include <iostream>
#include <errno.h>

Server::Server(const Config::map &config_map) throw(std::exception):
	_port(std::atoi(config_map.at(Config::KEYS[0])[0].c_str())),
	_names(config_map.at(Config::KEYS[1])),
	_root(config_map.at(Config::KEYS[2])[0]),
	_indexes(config_map.at(Config::KEYS[3])),
	_default_error_page(config_map.at(Config::KEYS[4])[0]),
	_client_max_body_size(std::atoi(config_map.at(Config::KEYS[5])[0].c_str())),
	_locations(initLocations(config_map)),
	_socket(ft::shared_ptr<Socket>(NULL))
{
	if (Server::isValidPort(_port) == false) {
		throw (InvalidPortException());
	}
	if (Server::isValidClientMaxBodySize(_client_max_body_size) == false) {
		throw (InvalidClientMaxBodySizeException());
	}

	try {
		this->_socket = ft::shared_ptr<Socket>(new Socket(this->_port));
	} catch (const std::exception &e) {
		std::cerr << "\033[31m" << "Error: " << e.what() << "\033[0m" << '\n';
		throw (FailToCreateSocketException());
	}
}
Server::~Server(void) {}

// Static
bool	Server::isValidPort(int port) { return (0 <= port && port <= 65535); }
bool	Server::isValidClientMaxBodySize(int client_max_body_size) { return (0 <= client_max_body_size); }

std::vector<Location>	Server::initLocations(const Config::map &config_map) {
	std::vector<Location>	locations;

	for (size_t i=0; i<config_map.at(std::string("location_") + Config::LOCATION_KEYS[0]).size(); i++) {
		try {
			locations.push_back(Location(config_map, i));
		} catch (const std::exception &e) {
			std::cerr << "\033[31m" << "Error: " << e.what() << "\033[0m" << '\n';
			throw (FailToInitializeLocationException());
		}
	}
	return (locations);
}

// Getters
const std::string				&Server::getDefaultErrorPage(void) const { return (this->_default_error_page); }
int								Server::getPort(void) const { return (this->_port); }
const std::vector<std::string>	&Server::getNames(void) const { return (this->_names); }
int								Server::getClientMaxBodySize(void) const { return (this->_client_max_body_size); }
const std::string				&Server::getRoot(void) const { return (this->_root); }
const std::vector<std::string>	&Server::getIndexes(void) const { return (this->_indexes); }
const std::vector<Location>		&Server::getLocations(void) const { return (this->_locations); }
const ft::shared_ptr<Socket>	&Server::getSocket(void) const { return (this->_socket); }

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
	for (size_t i=0; i<rhs.getLocations().size(); i++) {
		os << rhs.getLocations()[i] << '\n';
	}
	return (os);
}

// Exception
const char	*Server::InvalidPortException::what(void) const throw() { return ("Server: Invalid port"); }
const char	*Server::InvalidClientMaxBodySizeException::what(void) const throw() { return ("Server: Invalid client_max_body_size"); }
const char	*Server::FailToInitializeLocationException::what(void) const throw() { return ("Server: Fail to initialize location"); }
const char	*Server::FailToCreateSocketException::what(void) const throw() { return ("Server: Fail to create socket"); }
const char	*Server::FailToRunException::what(void) const throw() { return ("Server: Fail to run"); }