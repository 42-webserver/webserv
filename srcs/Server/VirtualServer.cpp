#include "../../incs/Server/VirtualServer.hpp"
#include "../../incs/Log/Logger.hpp"
#include <unistd.h>
#include <iostream>
#include <errno.h>

VirtualServer::VirtualServer(const Config::map &config_map) throw(std::exception):
	_root(_parseRoot(config_map)),
	_indexes(_parseIndexes(config_map)),
	_default_error_page(_parseDefaultErrorPage(config_map)),
	_client_max_body_size(_parseClientMaxBodySize(config_map)),
	_locations(_parseLocations(config_map))
	{}
VirtualServer::~VirtualServer(void) {}
VirtualServer::VirtualServer(const VirtualServer &ref):
	_root(ref._root),
	_indexes(ref._indexes),
	_default_error_page(ref._default_error_page),
	_client_max_body_size(ref._client_max_body_size),
	_locations(ref._locations)
	{}
VirtualServer	&VirtualServer::operator=(const VirtualServer &rhs) {
	if (this != &rhs) {
		this->~VirtualServer();
		new (this) VirtualServer(rhs);
	}
	return (*this);
}

std::string					VirtualServer::_parseRoot(const Config::map &config_map) {
	std::string	ret = config_map.at(Config::KEYS[Config::KEY::ROOT])[0];

	if (_rootIsValid(ret) == false) {
		throw (InvalidRootException());
	}

	return (ret);
}
std::vector<std::string>	VirtualServer::_parseIndexes(const Config::map &config_map) {
	std::vector<std::string>	ret = config_map.at(Config::KEYS[Config::KEY::INDEX]);

	if (_indexesIsValid(ret) == false) {
		throw (InvalidIndexesException());
	}
	return (ret);
}
std::string					VirtualServer::_parseDefaultErrorPage(const Config::map &config_map) {
	std::string	ret = config_map.at(Config::KEYS[Config::KEY::ERROR_PAGE])[0];

	if (_defaultErrorPageIsValid(ret) == false) {
		throw (InvalidDefaultErrorPageException());
	}
	return (ret);
}
int							VirtualServer::_parseClientMaxBodySize(const Config::map &config_map) {
	int	ret = std::atoi(config_map.at(Config::KEYS[Config::KEY::CLIENT_MAX_BODY_SIZE])[0].c_str());

	if (_clientMaxBodySizeIsValid(ret) == false) {
		throw (InvalidClientMaxBodySizeException());
	}

	return (ret);
}
std::vector<Location>		VirtualServer::_parseLocations(const Config::map &config_map) {
	std::vector<Location>	ret;

	for (size_t i=0; i<config_map.at(std::string("location_") + Config::LOCATION_KEYS[Config::LOCATION_KEY::DIR]).size(); i++) {
		try {
			ret.push_back(Location(config_map, i));
		} catch (const std::exception &e) {
			Logger::getInstance().error(e.what());
			throw (FailToInitializeLocationException());
		}
	}
	return (ret);
}

// Custom For use!!
bool						VirtualServer::_rootIsValid(const std::string &root) { return (true); (void) root; }
bool						VirtualServer::_indexesIsValid(const std::vector<std::string> &indexes) { return (true); (void) indexes; }
bool						VirtualServer::_defaultErrorPageIsValid(const std::string &default_error_page) { return (true); (void) default_error_page; }
bool						VirtualServer::_clientMaxBodySizeIsValid(const int client_max_body_size) { return (true); (void) client_max_body_size; }
bool						VirtualServer::_locationsIsValid(const std::vector<Location> &locations) { return (true); (void) locations; }
// Custom For use!!

// Getters
const std::string				&VirtualServer::getDefaultErrorPage(void) const { return (this->_default_error_page); }
int								VirtualServer::getClientMaxBodySize(void) const { return (this->_client_max_body_size); }
const std::string				&VirtualServer::getRoot(void) const { return (this->_root); }
const std::vector<std::string>	&VirtualServer::getIndexes(void) const { return (this->_indexes); }
const std::vector<Location>		&VirtualServer::getLocations(void) const { return (this->_locations); }

// Exception
const char	*VirtualServer::InvalidRootException::what(void) const throw() { return ("VirtualServer: Invalid root"); }
const char	*VirtualServer::InvalidIndexesException::what(void) const throw() { return ("VirtualServer: Invalid indexes"); }
const char	*VirtualServer::InvalidDefaultErrorPageException::what(void) const throw() { return ("VirtualServer: Invalid default_error_page"); }
const char	*VirtualServer::InvalidClientMaxBodySizeException::what(void) const throw() { return ("VirtualServer: Invalid client_max_body_size"); }
const char	*VirtualServer::FailToInitializeLocationException::what(void) const throw() { return ("VirtualServer: Fail to initialize location"); }
const char	*VirtualServer::InvalidLocationException::what(void) const throw() { return ("VirtualServer: Invalid location"); }

// Operator overload
std::ostream	&operator<<(std::ostream &os, const VirtualServer &rhs) {
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