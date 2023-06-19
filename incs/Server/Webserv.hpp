#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include "../../libs/shared_ptr/shared_ptr.hpp"
# include "../Config/Config.hpp"
# include "../Server/PhysicalServer.hpp"
# include "../Event/Event.hpp"
# include "../Event/ListenEvent.hpp"
# include "../Event/ReadEvent.hpp"
# include "../Event/WriteEvent.hpp"
# include "../EventQueue/EventQueue.hpp"
# include <iostream>
# include <map>
# include "./PhysicalServerManager.hpp"
class Webserv {

	private:
		static const int	MAX_SERVERS = 8;

	private:
		PhysicalServerManager	_physicalServerManager;

	public:
		Webserv(void);
		~Webserv(void);
		Webserv	&operator=(const Webserv &rhs);

	private:
		bool	_build(const Config &config) throw(std::exception);

	public:
		bool	run(const Config &config) throw(std::exception);

	public:
		class TooManyServersException: public std::exception {
			public:
				virtual const char* what() const throw();
		};
		class FailToBuildException: public std::exception {
			public:
				virtual const char* what() const throw();
		};
		class FailToRunException: public std::exception {
			public:
				virtual const char* what() const throw();
		};
};

// Todo:
// std::ostream	&operator<<(std::ostream &os, const Webserv &webserv);

#endif