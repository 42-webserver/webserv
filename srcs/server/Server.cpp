#include "../../incs/server/Server.hpp"
#include "../../incs/config/Config.hpp"
#include <unistd.h>
#include <iostream>
#include <errno.h>

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
	} catch (const Socket::FailToCreateException &e) {
		throw (Socket::FailToCreateException());
	} catch (const Socket::FailToBindException &e) {
		throw (Socket::FailToBindException());
	} catch (const Socket::FailToListenException &e) {
		throw (Socket::FailToListenException());
	} catch (const Kqueue::FailToCreateException &e) {
		throw (Kqueue::FailToCreateException());
	} catch (const Kqueue::FailToControlException &e) {
		throw (Kqueue::FailToControlException());
	} catch (...) {
		throw (Server::UnknownErrorException());
	}
}
Server::~Server(void) {}

// Static
bool	Server::_is_valid_port(int port) { return (0 <= port && port <= 65535); }
bool	Server::_is_valid_client_max_body_size(int client_max_body_size) { return (0 <= client_max_body_size); }

// Utils
int	Server::accept(void) throw(std::exception) {
	int	client_fd;

	try {
		client_fd = this->_socket.accept();
	} catch (const Socket::FailToAcceptException &e) {
		throw (Socket::FailToAcceptException());
	} catch (const Socket::FailToControlException &e) {
		throw (Socket::FailToControlException());
	} catch (...) {
		throw (Server::UnknownErrorException());
	}
	return (client_fd);
}

int	Server::read(int event_fd, char *buf) {
	int	read_size;

	try {
		read_size = ::read(event_fd, buf, Socket::MAX_SIZE);
		if (read_size == -1) {
			this->_kqueue.deleteEvent(event_fd);
			throw (Server::FailToReadException());
		}
	} catch (const Server::FailToReadException &e) {
		throw (Server::FailToReadException());
	} catch (...) {
		throw (Server::UnknownErrorException());
	}
	buf[read_size] = '\0';
	return (read_size);
}

int Server::send(int event_fd) {
	int		send_size;
	char	response[Socket::MAX_SIZE] = "HTTP/1.1 200 OK\r\nContent-Length: 12\r\n\r\nHello World!";

	try {
		send_size = ::send(event_fd, response, Socket::MAX_SIZE, 0);
		if (send_size == -1) {
			throw (Server::FailToSendException());
		}
	} catch (const Server::FailToSendException &e) {
		throw (Server::FailToSendException());
	} catch (...) {
		throw (Server::UnknownErrorException());
	}
	return (send_size);
}

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
	while (true) {
		int	event_length = this->_kqueue.length();
		
		if (event_length == -1) {
			throw (Kqueue::FailToGetEventException());
		}

		for (int i=0; i<event_length; i++) {
			int	event_fd = this->_kqueue.getEventFd(i);

			if (event_fd == this->_socket.getFd()) {
				int	client_fd;

				try {
					client_fd = this->accept();
					this->_kqueue.addEvent(client_fd);
				} catch (const Socket::FailToAcceptException &e) {
					throw (Socket::FailToAcceptException());
				} catch (const Socket::FailToControlException &e) {
					throw (Socket::FailToControlException());
				} catch (const Kqueue::FailToControlException &e) {
					throw (Kqueue::FailToControlException());
				} catch (...) {
					throw (Server::UnknownErrorException());
				}
			} else {
				int		read_size, send_size;
				char	buf[Socket::MAX_SIZE];

				try {
					if ((read_size = this->read(event_fd, buf)) == 0) {
						this->_kqueue.deleteEvent(event_fd);
					}
					printf("Read: Received Message size: %d\n", read_size);
					printf("%s\n", buf);
					send_size = this->send(event_fd);
					printf("Send: Message size: %d\n", send_size);
				} catch (const Server::FailToReadException &e) {
					throw (Server::FailToReadException());
				} catch (const Server::FailToSendException &e) {
					throw (Server::FailToSendException());
				} catch (...) {
					throw (Server::UnknownErrorException());
				}
			}
		}
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
const char	*Server::InvalidPortException::what(void) const throw() { return ("Server: Invalid port"); }
const char	*Server::InvalidClientMaxBodySizeException::what(void) const throw() { return ("Server: Invalid client_max_body_size"); }
const char	*Server::FailToReadException::what(void) const throw() { return ("Server: Fail to read"); }
const char	*Server::FailToSendException::what(void) const throw() { return ("Server: Fail to send"); }
const char	*Server::UnknownErrorException::what(void) const throw() { return ("Server: Unknown error"); }