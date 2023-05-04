#ifndef SERVER_HPP
# define SERVER_HPP

# include "../config/Config.hpp"
# include "../socket/Socket.hpp"
# include "../kqueue/Kqueue.hpp"
# include <string>
# include <vector>
# include <iostream>
# include <map>

class Server {
	private:
		static const int				MAX_EVENTS = 16;

	private:
		const int						_port;
		const std::vector<std::string>	_names;
		const std::string				_root;
		const std::vector<std::string>	_indexes;
		const std::string				_default_error_page;
		const int						_client_max_body_size;
		const std::string				_cgi_pass;
		//std::string					_cgi_extension;
		//std::string					_cgi_path;
		//TODO: routing rules (location, redirect)

	// Static	
	private:
		static bool						_is_valid_port(int port);
		static bool						_is_valid_client_max_body_size(int client_max_body_size);

	private:
		Socket							_socket;
		Kqueue							_kqueue;

	// Utils
	private:
		int	accept(void) throw(std::exception);
		int	read(int event_fd, char *buf);
		int	send(int event_fd);

	// Constructor & Destructor
	public:
		Server(const Config::map &config_map) throw(std::exception);
		~Server(void);

	// Getters
	public:
		const std::string				&getDefaultErrorPage(void) const;
		int								getPort(void) const;
		const std::vector<std::string>	&getNames(void) const;
		int								getClientMaxBodySize(void) const;
		const std::string				&getRoot(void) const;
		const std::vector<std::string>	&getIndexes(void) const;
		const std::string				&getCgiPass(void) const;
		//const std::string				&getCgiExtension(void) const;
		//const std::string				&getCgiPath(void) const;
	
	// Utils
	public:
		bool	init(void) throw(std::exception);

	// Exception
	public:
		class InvalidPortException: public std::exception {
			public:
				virtual const char *what() const throw();
		};

		class InvalidClientMaxBodySizeException: public std::exception {
			public:
				virtual const char *what() const throw();
		};

		class FailToReadException: public std::exception {
			public:
				virtual const char *what() const throw();
		};

		class FailToSendException: public std::exception {
			public:
				virtual const char *what() const throw();
		};

		class UnknownErrorException: public std::exception {
			public:
				virtual const char *what() const throw();
		};
};

std::ostream	&operator<<(std::ostream &os, const Server &rhs);

#endif