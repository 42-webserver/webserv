#include "../../incs/server/Server.hpp"
#include "../../incs/config/Config.hpp"

Server::Server(const Config::map &config_map) throw(std::exception):
	_port(std::atoi(config_map.find(Config::PORT)->second[0].c_str())),
	_names(config_map.find(Config::SERVER_NAME)->second),
	_root(config_map.find(Config::ROOT)->second[0]),
	_indexes(config_map.find(Config::INDEX)->second),
	_default_error_page(config_map.find(Config::ERROR_PAGE)->second[0]),
	_client_max_body_size(std::atoi(config_map.find(Config::CLIENT_MAX_BODY_SIZE)->second[0].c_str())),
	_cgi_pass(config_map.find(Config::CGI_PASS)->second[0]),
	_socket(Socket()),
	_kqueue(Kqueue())
{
	if (Server::_is_valid_port(_port) == false) {
		throw (Server::InvalidPortException());
	}
	if (Server::_is_valid_client_max_body_size(_client_max_body_size) == false) {
		throw (Server::InvalidClientMaxBodySizeException());
	}
	try {
		this->_socket = Socket(this->_port);
		this->_kqueue = Kqueue(this->_socket.getFd());
	} catch (const Socket::SocketCreationErrorException &e) {
		throw (Socket::SocketCreationErrorException());
	} catch (const Kqueue::KqueueCreationErrorException &e) {
		throw (Kqueue::KqueueCreationErrorException());
	} catch (...) {
		throw (Server::UnknownErrorException());
	}
}
Server::~Server(void) {}

// Static
bool	Server::_is_valid_port(int port) { return (0 <= port && port <= 65535); }
bool	Server::_is_valid_client_max_body_size(int client_max_body_size) { return (0 <= client_max_body_size); }

// Getters
const std::string				&Server::getDefaultErrorPage(void) const { return (this->_default_error_page); }
int								Server::getPort(void) const { return (this->_port); }
const std::vector<std::string>	&Server::getNames(void) const { return (this->_names); }
int								Server::getClientMaxBodySize(void) const { return (this->_client_max_body_size); }
const std::string				&Server::getRoot(void) const { return (this->_root); }
const std::vector<std::string>	&Server::getIndexes(void) const { return (this->_indexes); }
const std::string				&Server::getCgiPass(void) const { return (this->_cgi_pass); }

// Util
bool	Server::init(void) throw(std::exception) {
	try {
		this->_socket.bind();
		this->_socket.listen();
	} catch (const Socket::SocketBindErrorException &e) {
		throw (Socket::SocketBindErrorException());
	} catch (const Socket::SocketListenErrorException &e) {
		throw (Socket::SocketListenErrorException());
	} catch (...) {
		throw (Server::UnknownErrorException());
	}
	return (true);
}

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
const char	*Server::InvalidPortException::what(void) const throw() { return ("Invalid port"); }
const char	*Server::InvalidClientMaxBodySizeException::what(void) const throw() { return ("Invalid client_max_body_size"); }
const char	*Server::UnknownErrorException::what(void) const throw() { return ("Unknown error"); }