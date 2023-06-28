#include "../../incs/Server/Location.hpp"

// Constructor & Destructor
Location::Location(void): _dir(""), _root(""), _alias(""), _autoindex(""), _return_status(""), _return_url("") {}
Location::Location(const Config::map &config_map, size_t idx): _dir(_parseDir(config_map, idx)), _root(_parseRoot(config_map, idx)), _alias(_parseAlias(config_map, idx)), _autoindex(_parseAutoIndex(config_map, idx)), _return_status(_parseReturnStatus(config_map, idx)), _return_url(_parseReturnUrl(config_map, idx)) {}
Location::Location(const Location &ref): _dir(ref._dir), _root(ref._root), _alias(ref._alias), _autoindex(ref._autoindex), _return_status(ref._return_status), _return_url(ref._return_url) {}
Location::~Location(void) {}
Location	&Location::operator=(const Location &rhs) {
	if (this != &rhs) {
		this->~Location();
		new (this) Location(rhs);
	}
	return (*this);
}

std::string		Location::_parseDir(const Config::map &config_map, size_t idx) throw(std::exception) {
	std::string	ret = config_map.at(std::string("location_") + Config::LOCATION_KEYS[Config::LOCATION_KEY::DIR])[idx];

	if (_dirIsValid(ret) == false) {
		throw (InvalidDirException());
	}
	return (ret);
}
std::string		Location::_parseRoot(const Config::map &config_map, size_t idx) throw(std::exception) {
	std::string	ret = config_map.at(std::string("location_") + Config::LOCATION_KEYS[Config::LOCATION_KEY::ROOT])[idx];

	if (_rootIsValid(ret) == false) {
		throw (InvalidRootException());
	}
	return (ret);
}
std::string		Location::_parseAlias(const Config::map &config_map, size_t idx) throw(std::exception) {
	std::string	ret = config_map.at(std::string("location_") + Config::LOCATION_KEYS[Config::LOCATION_KEY::ALIAS])[idx];

	if (_aliasIsValid(ret) == false) {
		throw (InvalidAliasException());
	}
	return (ret);
}
std::string		Location::_parseAutoIndex(const Config::map &config_map, size_t idx) throw(std::exception) {
	std::string	ret = config_map.at(std::string("location_") + Config::LOCATION_KEYS[Config::LOCATION_KEY::AUTO_INDEX])[idx];

	if (_autoIndexIsValid(ret) == false) {
		throw (InvalidAutoIndexException());
	}
	return (ret);
}
std::string		Location::_parseReturnStatus(const Config::map &config_map, size_t idx) throw(std::exception) {
	std::string	ret = config_map.at(std::string("location_") + Config::LOCATION_KEYS[Config::LOCATION_KEY::RETURN_STATUS])[idx];

	if (_returnStatusIsValid(ret) == false) {
		throw (InvalidReturnStatusException());
	}
	return (ret);
}
std::string		Location::_parseReturnUrl(const Config::map &config_map, size_t idx) throw(std::exception) {
	std::string	ret = config_map.at(std::string("location_") + Config::LOCATION_KEYS[Config::LOCATION_KEY::RETURN_URL])[idx];

	if (_returnUrlIsValid(ret) == false) {
		throw (InvalidReturnUrlException());
	}
	return (ret);
}
bool			Location::_dirIsValid(const std::string &dir) { return (dir == "" || dir[0] == '/'); }
bool			Location::_rootIsValid(const std::string &root) { return (root == "" || root[0] == '/'); }
bool			Location::_aliasIsValid(const std::string &alias) { return (alias == "" || alias[0] == '/'); }
bool			Location::_autoIndexIsValid(const std::string &autoindex) { return (autoindex == "" || autoindex == "on" || autoindex == "off"); }
bool			Location::_returnStatusIsValid(const std::string &return_status) { return ( return_status == "" || return_status == "301" || return_status == "302"); }
bool			Location::_returnUrlIsValid(const std::string &return_url) { return (return_url == "" || return_url[0] == '/'); }

const std::string	&Location::getDir(void) const { return (this->_dir); }
const std::string	&Location::getRoot(void) const { return (this->_root); }
const std::string	&Location::getAlias(void) const { return (this->_alias); }
const std::string	&Location::getAutoIndex(void) const { return (this->_autoindex); }
const std::string	&Location::getReturnStatus(void) const { return (this->_return_status); }
const std::string	&Location::getReturnUrl(void) const { return (this->_return_url); }

const char	*Location::InvalidSyntaxException::what() const throw() { return ("Location: Invalid syntax"); }
const char	*Location::InvalidDirException::what() const throw() { return ("Location: Invalid dir"); }
const char	*Location::InvalidRootException::what() const throw() { return ("Location: Invalid root"); }
const char	*Location::InvalidAliasException::what() const throw() { return ("Location: Invalid alias"); }
const char	*Location::InvalidAutoIndexException::what() const throw() { return ("Location: Invalid autoindex"); }
const char	*Location::InvalidReturnStatusException::what() const throw() { return ("Location: Invalid return status"); }
const char	*Location::InvalidReturnUrlException::what() const throw() { return ("Location: Invalid return url"); }

std::ostream	&operator<<(std::ostream &os, const Location &location) {
	os << "\t\t\t\t\t\t\t\t\t" << "Location:" << '\n';
	os << "\t\t\t\t\t\t\t\t\t\t" << "dir: " << location._dir << '\n';
	if (location._root != "") {
		os << "\t\t\t\t\t\t\t\t\t\t" << "root: " << location._root << '\n';
	}
	if (location._alias != "") {
		os << "\t\t\t\t\t\t\t\t\t\t" << "alias: " << location._alias << '\n';
	}
	if (location._autoindex != "") {
		os << "\t\t\t\t\t\t\t\t\t\t" << "autoindex: " << location._autoindex << '\n';
	}
	if (location._return_status != "") {
		os << "\t\t\t\t\t\t\t\t\t\t" << "return_status: " << location._return_status << '\n';
	}
	if (location._return_url != "") {
		os << "\t\t\t\t\t\t\t\t\t\t" << "return_url: " << location._return_url << '\n';
	}
	return (os);
}