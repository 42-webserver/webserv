#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include "../../libs/shared_ptr/shared_ptr.hpp"
# include "../EventQueue/EventQueue.hpp"
# include "../Config/Config.hpp"
# include "../Event/Event.hpp"
# include "../Event/ListenEvent.hpp"
# include "../Event/ReadEvent.hpp"
# include "../Event/WriteEvent.hpp"
# include "Server.hpp"

class Webserv {
	public:
		typedef std::vector<Server>::iterator		iterator;
		typedef std::vector<Server>::const_iterator	const_iterator;

	private:
		static const int		MAX_SERVERS = 8;

	private:
		std::vector<Server>		_servers;

	public:
		Webserv(void);
		Webserv(const Config &config);
		Webserv(const Webserv &ref);
		~Webserv(void);
		Webserv	&operator=(const Webserv &rhs);
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