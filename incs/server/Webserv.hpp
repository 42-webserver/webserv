#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include "../../libs/shared_ptr/incs/shared_ptr.hpp"
# include "../kqueue/Kqueue.hpp"
# include "../config/Config.hpp"
# include "../client/Client.hpp"
# include "Server.hpp"

class Webserv {
	public:
		typedef std::vector<Server>::iterator		iterator;
		typedef std::vector<Server>::const_iterator	const_iterator;

	private:
		static const int		MAX_SERVERS = 8;

	private:
		ft::shared_ptr<Kqueue>	_kqueue;
		std::vector<Server>		_servers;

	public:
		Webserv(void);
		Webserv(const Config &config);
		Webserv(const Webserv &ref);
		~Webserv(void);
		Webserv	&operator=(const Webserv &rhs);
	
	private:
		const_iterator	findServer(int event_fd) const;
		int				read(int client_fd, Client *client, char *buf);
		int				send(int client_fd, Client *client);

	public:
		bool	run(void) throw(std::exception);

	public:
		class TooManyServersException: public std::exception {
			public:
				virtual const char* what() const throw();
		};
		class FailToConstructException: public std::exception {
			public:
				virtual const char* what() const throw();
		};
		class FailToRunException: public std::exception {
			public:
				virtual const char* what() const throw();
		};
};

#endif