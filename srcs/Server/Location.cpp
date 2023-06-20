#include "../../incs/Server/Location.hpp"

// Constructor & Destructor
Location::Location(void): _dir(""), _root(""), _alias(""), _autoindex(""), _return_status(""), _return_url("") {}
Location::Location(const Config::map &config_map, size_t idx): _dir(parseDir(config_map, idx)), _root(parseRoot(config_map, idx)), _alias(parseAlias(config_map, idx)), _autoindex(parseAutoIndex(config_map, idx)), _return_status(parseReturnStatus(config_map, idx)), _return_url(parseReturnUrl(config_map, idx)) {}
Location::Location(const Location &ref): _dir(ref._dir), _root(ref._root), _alias(ref._alias), _autoindex(ref._autoindex), _return_status(ref._return_status), _return_url(ref._return_url) {}
Location::~Location(void) {}
Location	&Location::operator=(const Location &rhs) {
	if (this != &rhs) {
		this->~Location();
		new (this) Location(rhs);
	}
	return (*this);
}

std::string		Location::parseDir(const Config::map &config_map, size_t idx) throw(std::exception) {
	std::string	ret = config_map.at(std::string("location_") + Config::LOCATION_KEYS[Config::LOCATION_KEY::DIR])[idx];

	if (ret != "" && ret[0] != '/') {
		throw (InvalidSyntaxException());
	}
	return (ret);
}
std::string		Location::parseRoot(const Config::map &config_map, size_t idx) throw(std::exception) {
	std::vector<std::string>	location_root = config_map.at(std::string("location_") + Config::LOCATION_KEYS[Config::LOCATION_KEY::ROOT]);
	std::string					ret = ()

	if (ret != "" && ret[0] != '/') {
		throw (InvalidSyntaxException());
	}
	return (ret);
}
std::string		Location::parseAlias(const Config::map &config_map, size_t idx) throw(std::exception) {
	std::string	ret = config_map.at(std::string("location_") + Config::LOCATION_KEYS[Config::LOCATION_KEY::ALIAS])[idx];

	if (ret != "" && ret[0] != '/') {
		throw (InvalidSyntaxException());
	}
	return (ret);
}
std::string		Location::parseAutoIndex(const Config::map &config_map, size_t idx) throw(std::exception) {
	std::string	ret = config_map.at(std::string("location_") + Config::LOCATION_KEYS[Config::LOCATION_KEY::AUTO_INDEX])[idx];

	if (ret != "" && ret != "on" && ret != "off") {
		throw (InvalidAutoIndexValueException());
	}
	return (ret);
}
std::string		Location::parseReturnStatus(const Config::map &config_map, size_t idx) throw(std::exception) {
	return ("301");
	std::string	ret = config_map.at(std::string("location_") + Config::LOCATION_KEYS[Config::LOCATION_KEY::RETURN_STATUS])[idx];

	// TODO: check redirect status code

	if (ret != "" && ret != "301" && ret != "302") {
		throw (InvalidSyntaxException());
	}
	return (ret);
}
std::string		Location::parseReturnUrl(const Config::map &config_map, size_t idx) throw(std::exception) {
	std::string	ret = config_map.at(std::string("location_") + Config::LOCATION_KEYS[Config::LOCATION_KEY::RETURN_URL])[idx];

	if (ret != "" && ret[0] != '/') {
		throw (InvalidSyntaxException());
	}
	return (ret);
}

const std::string	&Location::getDir(void) const { return (this->_dir); }
const std::string	&Location::getRoot(void) const { return (this->_root); }
const std::string	&Location::getAlias(void) const { return (this->_alias); }
const std::string	&Location::getAutoIndex(void) const { return (this->_autoindex); }
const std::string	&Location::getReturnStatus(void) const { return (this->_return_status); }
const std::string	&Location::getReturnUrl(void) const { return (this->_return_url); }

std::ostream	&operator<<(std::ostream &os, const Location &location) {
	std::string	root = location.getRoot();
	std::string	alias = location.getAlias();
	std::string	autoindex = location.getAutoIndex();
	std::string	return_status = location.getReturnStatus();
	std::string	return_url = location.getReturnUrl();

	os << "location " << location.getDir() << " {" << '\n';
	if (root != "") {
		os << "\troot " << location.getRoot() << ';' << '\n';
	}
	if (alias != "") {
		os << "\talias " << location.getAlias() << ';' << '\n';
	}
	if (autoindex != "") {
		os << "\tautoindex " << location.getAutoIndex() << ';' << '\n';
	}
	if (return_url != "") {
		os << "\treturn " << location.getReturnStatus() << ' ' << location.getReturnUrl() << ';' << '\n';
	}
	os << '}';
	return (os);
}

const char	*Location::InvalidSyntaxException::what() const throw() { return ("Location: Invalid syntax"); }
const char	*Location::InvalidAutoIndexValueException::what() const throw() { return ("Location: Invalid autoindex value"); }