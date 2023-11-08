#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include "../Config/Config.hpp"
# include "PhysicalServerManager.hpp"
# include <iostream>

class Webserv {
	private:
		PhysicalServerManager	_physical_server_manager;

	public:
		Webserv(void);
		~Webserv(void);
		Webserv(const Webserv &ref);
		Webserv	&operator=(const Webserv &rhs);

	public:
		bool	run(const Config &config);

	public:
		class FailToRunException: public std::exception {
			public:
				virtual const char* what() const throw();
		};

	friend std::ostream	&operator<<(std::ostream &os, const Webserv &webserv);
};

//std::ostream	&operator<<(std::ostream &os, const Webserv &webserv);

#endif