#include "../../incs/server/Server.hpp"

std::string	Server::_default_err_page = "default_err_page";

Server::Server(void) {}
Server::~Server(void) {}

// Getters
const std::string				&Server::getDefaultErrPage(void) { return Server::_default_err_page; }
int								Server::getPort(void) const { return this->_port; }
const std::vector<std::string>	&Server::getNames(void) const { return this->_names; }
int								Server::getClientMaxBodySize(void) const { return this->_client_max_body_size; }
const std::string				&Server::getRoot(void) const { return this->_root; }
const std::vector<std::string>	&Server::getIndexes(void) const { return this->_indexes; }
const std::string				&Server::getCgiPass(void) const { return this->_cgi_pass; }
//const std::string	&Server::getCgiExtension(void) const { return this->_cgi_extension; }
//const std::string	&Server::getCgiPath(void) const { return this->_cgi_path; }

// Setters
bool	Server::setPort(int port) throw(std::exception) {
	if (port < 0 || 65535 < port) {
		throw (Server::InvalidPortException());
	}
	this->_port = port;
	return (true);
}

bool	Server::setClientMaxBodySize(int client_max_body_size) throw(std::exception) {
	if (client_max_body_size < 0) {
		throw (Server::InvalidClientMaxBodySizeException());
	}
	this->_client_max_body_size = client_max_body_size;
	return (true);
}

bool	Server::setRoot(const std::string &root) {
	this->_root = root;
	return (true);
}

bool	Server::setCgiPass(const std::string &cgi_pass) {
	this->_cgi_pass = cgi_pass;
	return (true);
}

bool	Server::setIndex(const std::string &index) {
	this->_indexes.push_back(index);
	return (true);
}

//bool	Server::setCgiExtension(const std::string &cgi_extension) {
//	this->_cgi_extension = cgi_extension;
//	return (true);
//}

//bool	Server::setCgiPath(const std::string &cgi_path) {
//	this->_cgi_path = cgi_path;
//	return (true);
//}

// Utils
bool	Server::findName(const std::string &name) const {
	return (std::find(this->_names.begin(), this->_names.end(), name) != this->_names.end());
}

bool	Server::pushName(const std::string &name) {
	if (this->findName(name)) {
		return (true);
	}
	this->_names.push_back(name);
	return (true);
}

bool	Server::findIndex(const std::string &index) const {
	return (std::find(this->_indexes.begin(), this->_indexes.end(), index) != this->_indexes.end());
}

bool	Server::pushIndex(const std::string &index) {
	this->_indexes.push_back(index);
	return (true);
}

// Exception
const char *Server::InvalidPortException::what() const throw() { return ("Invalid port"); }
const char *Server::InvalidClientMaxBodySizeException::what() const throw() { return ("Invalid client_max_body_size"); }

std::ostream	&operator<<(std::ostream &os, const Server &rhs) {
	os << "Port: " << rhs.getPort() << '\n';
	os << "Names: ";
	for (std::vector<std::string>::const_iterator it = rhs.getNames().begin(); it != rhs.getNames().end(); it++) {
		os << *it << ' ';
	}
	os << '\n';
	os << "ClientMaxBodySize: " << rhs.getClientMaxBodySize() << '\n';
	os << "Root: " << rhs.getRoot() << '\n';
	os << "Indexed: ";
	for (std::vector<std::string>::const_iterator it = rhs.getIndexes().begin(); it != rhs.getIndexes().end(); it++) {
		os << *it << ' ';
	}
	os << '\n';
	os << "CgiPass: " << rhs.getCgiPass() << '\n';
	//os << "CgiExtension: " << rhs.getCgiExtension() << '\n';
	//os << "CgiPath: " << rhs.getCgiPath() << '\n';
	return (os);
}