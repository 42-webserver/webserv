#include "../../incs/server/Webserv.hpp"
#include <unistd.h>

Webserv::Webserv(void): _kqueue(ft::shared_ptr<Kqueue>(new Kqueue())), _servers(std::vector<Server>()) {}
Webserv::Webserv(const Config &config): _kqueue(ft::shared_ptr<Kqueue>(new Kqueue())), _servers(std::vector<Server>()) {
	std::vector< Config::map >	server_configs = config.getConfigMaps();

	if (MAX_SERVERS <= server_configs.size()) {
		throw (TooManyServersException());
	}

	for (size_t i=0; i<server_configs.size(); ++i) {
		try {
			this->_servers.push_back(Server(server_configs[i]));
			this->_kqueue->addEvent(this->_servers.back().getSocket()->getFd(), &this->_servers.back());
		} catch (const std::exception &e) {
			std::cerr << "\033[31m" << "Error: " << e.what() << "\033[0m" << '\n';
			throw (FailToConstructException());
		}
	}
}
Webserv::Webserv(const Webserv &ref): _kqueue(ref._kqueue), _servers(ref._servers) {}
Webserv::~Webserv(void) {}
Webserv	&Webserv::operator=(const Webserv &rhs) {
	if (this != &rhs) {
		this->~Webserv();
		new (this) Webserv(rhs);
	}
	return (*this);
}

Webserv::const_iterator	Webserv::findServer(int event_fd) const {
	std::vector<Server>::const_iterator	it = this->_servers.begin();
	std::vector<Server>::const_iterator	ite = this->_servers.end();

	while (it != ite) {
		if (event_fd == it->getSocket()->getFd()) {
			return (it);
		}
		++it;
	}
	return (ite);
}
int						Webserv::read(int client_fd, Client *client, char *buf) {
	int	read_size = ::read(client_fd, buf, Socket::MAX_SIZE);

	// Request is not complete
	if (read_size == -1) {
		return (read_size);
	}
	
	// Client closed connection
	if (read_size == 0) {
		try {
			this->_kqueue->deleteEvent(client_fd, client);
		} catch (const std::exception &e) {
			std::cerr << "\033[31m" << "Error: " << e.what() << "\033[0m" << '\n';
		}
	}

	buf[read_size] = '\0';
	return (read_size);
}
int 					Webserv::send(int client_fd, Client *client) {
	// Response to test
	const char	response[Socket::MAX_SIZE] = "HTTP/1.1 200 OK\r\nContent-Length: 12\r\n\r\nHello World!";
	int			send_size = ::send(client_fd, response, Socket::MAX_SIZE, 0);

	if (send_size < 0) {
		try {
			this->_kqueue->deleteEvent(client_fd, client);
		} catch (const std::exception &e) {
			std::cerr << "\033[31m" << "Error: " << e.what() << "\033[0m" << '\n';
		}
	}
	return (send_size);
}
bool					Webserv::run(void) throw(std::exception) {
	// Run server
	while (true) {
		int	event_length;
		
		// Polling event
		try {
			event_length = this->_kqueue->length();
		} catch (const std::exception &e) {
			std::cerr << "\033[31m" << "Error: " << e.what() << "\033[0m" << '\n';
			throw (FailToRunException());
		}

		// Handle event
		for (int i=0; i<event_length; i++) {
			int				event_fd = this->_kqueue->getEventFd(i);
			Data			*event_data = (Data *) this->_kqueue->getEventData(i);
			const_iterator	it = this->findServer(event_fd);

			// New connection (accept)
			if (it != this->_servers.end()) {
				int	client_fd = it->accept();

				if (client_fd == -1) {
					continue;
				}
				try {
					this->_kqueue->addEvent(client_fd, new Client(client_fd));
				} catch (const std::exception &e) {
					std::cerr << "\033[31m" << "Error: " << e.what() << "\033[0m" << '\n';
				}
			} else { // Known connection (read, send)
				int		read_size, send_size;
				char	buf[Socket::MAX_SIZE];

				// Read HTTP Request
				if ((read_size = this->read(event_fd, dynamic_cast<Client *>(event_data), buf)) <= 0) {
					continue;
				}
				printf("Read: Received Message size: %d\n", read_size);
				printf("%s", buf);
				
				// Parse HTTP Request
				
				// Build Response
				
				// Send HTTP Response
				if ((send_size = this->send(event_fd, dynamic_cast<Client *>(event_data))) < 0) {
					continue;
				}
				printf("Send: Message size: %d\n\n", send_size);
			}
		}
	}
	return (true);
}

const char	*Webserv::TooManyServersException::what() const throw() { return ("Webserv: Too many server_configs"); }
const char	*Webserv::FailToConstructException::what() const throw() { return ("Webserv: Fail to construct"); }
const char	*Webserv::FailToRunException::what() const throw() { return ("Webserv: Fail to run"); }