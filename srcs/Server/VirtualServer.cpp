#include "../../incs/Server/VirtualServer.hpp"
#include <unistd.h>
#include <iostream>
#include <errno.h>

VirtualServer::VirtualServer(const Config::map &config_map) throw(std::exception):
	_root(_initRoot(config_map)),
	_indexes(_initIndexes(config_map)),
	_default_error_page(_initDefaultErrorPage(config_map)),
	_client_max_body_size(_initClientMaxBodySize(config_map)),
	_locations(_initLocations(config_map))
	{}
VirtualServer::~VirtualServer(void) {}

std::string					VirtualServer::_initRoot(const Config::map &config_map) {
	std::string	ret = config_map.at(Config::KEYS[2])[0];

	// if (_rootIsValid(ret) == false) {
	// 	throw (FailToInitializeLocationException());
	// }

	return (ret);
}
std::vector<std::string>	VirtualServer::_initIndexes(const Config::map &config_map) {
	std::vector<std::string>	ret = config_map.at(Config::KEYS[3]);

	// if (_indexesIsValid(ret) == false) {
	// 	throw (FailToInitializeLocationException());
	// }

	return (ret);
}
std::string					VirtualServer::_initDefaultErrorPage(const Config::map &config_map) {
	std::string	ret = config_map.at(Config::KEYS[4])[0];

	// if (_defaultErrorPageIsValid(ret) == false) {
	// 	throw (FailToInitializeLocationException());
	// }

	return (ret);
}
int							VirtualServer::_initClientMaxBodySize(const Config::map &config_map) {
	int	ret = std::atoi(config_map.at(Config::KEYS[5])[0].c_str());

	// if (_clientMaxBodySizeIsValid(ret) == false) {
	// 	throw (FailToInitializeLocationException());
	// }

	return (ret);
}
std::vector<Location>		VirtualServer::_initLocations(const Config::map &config_map) {
	std::vector<Location>	ret;

	for (size_t i=0; i<config_map.at(std::string("location_") + Config::LOCATION_KEYS[0]).size(); i++) {
		try {
			ret.push_back(Location(config_map, i));
		} catch (const std::exception &e) {
			std::cerr << "\033[31m" << "Error: " << e.what() << "\033[0m" << '\n';
			throw (FailToInitializeLocationException());
		}
	}
	return (ret);
}

// Custom For use!!
// bool						VirtualServer::_rootIsValid(const std::string &root) { return (true); }
// bool						VirtualServer::_indexesIsValid(const std::vector<std::string> &indexes) { return (true); }
// bool						VirtualServer::_defaultErrorPageIsValid(const std::string &default_error_page) { return (true); }
// bool						VirtualServer::_clientMaxBodySizeIsValid(const int client_max_body_size) { return (true); }
// bool						VirtualServer::_locationsIsValid(const std::vector<Location> &locations) { return (true); }
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
const char	*VirtualServer::FailToRunException::what(void) const throw() { return ("VirtualServer: Fail to run"); }

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