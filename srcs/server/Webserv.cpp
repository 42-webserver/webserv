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
			this->_kqueue->addEvent(this->_servers.back().getSocket()->getFd(), new ListenEvent(this->_servers.back().getSocket()->getFd()));
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

int		Webserv::read(int client_fd, ReadEvent *read_event) {
	char	buf[ReadEvent::MAX_SIZE];
	int		read_size = ::read(client_fd, buf, ReadEvent::MAX_SIZE);

	// Request is not complete
	if (read_size == -1) {
		return (read_size);
	}
	
	// Client closed connection
	if (read_size == 0) {
		try {
			this->_kqueue->deleteEvent(client_fd, read_event);
		} catch (const std::exception &e) {
			std::cerr << "\033[31m" << "Error: " << e.what() << "\033[0m" << '\n';
		}
	} else {
		buf[read_size] = '\0';
		read_event->append(buf);
	}
	return (read_size);
}
int 	Webserv::send(int client_fd, WriteEvent *write_event) {
	// TODO: Take response from HTTPResponseHandler
	// Response to test
	const char	response[Socket::MAX_SIZE] = "HTTP/1.1 200 OK\r\nContent-Length: 12\r\n\r\nHello World!";
	int			send_size = ::send(client_fd, response, Socket::MAX_SIZE, 0);

	if (send_size < 0) {
		try {
			this->_kqueue->deleteEvent(client_fd, write_event);
		} catch (const std::exception &e) {
			std::cerr << "\033[31m" << "Error: " << e.what() << "\033[0m" << '\n';
		}
	}
	return (send_size);
}
bool	Webserv::run(void) throw(std::exception) {
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
			int			event_fd = this->_kqueue->getEventFd(i);
			Event		*event_data = (Event *) this->_kqueue->getEventData(i);
			ListenEvent	*listen_event = dynamic_cast<ListenEvent *>(event_data);
			ReadEvent	*read_event = dynamic_cast<ReadEvent *>(event_data);
			WriteEvent	*write_event = dynamic_cast<WriteEvent *>(event_data);

			// New connection (accept)
			if (listen_event != NULL) {
				int	client_fd = listen_event->accept();

				if (client_fd == -1) {
					continue;
				}
				try {
					this->_kqueue->addEvent(client_fd, new ReadEvent(client_fd));
				} catch (const std::exception &e) {
					std::cerr << "\033[31m" << "Error: " << e.what() << "\033[0m" << '\n';
				}
			} else { // Known connection (read, send)
				int		read_size, send_size;

				// Read HTTP Request
				if ((read_size = this->read(event_fd, read_event)) <= 0) {
					continue;
				}
				printf("Read: Received Message size: %d\n", read_size);
				for (size_t i=0; i<read_event->getBuffer().size(); ++i) {
					printf("%c", read_event->getBuffer()[i]);
				}
				
				// Parse HTTP Request
				
				// Build Response
				
				// Send HTTP Response
				if ((send_size = this->send(event_fd, write_event)) < 0) {
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