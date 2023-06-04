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
# include <map>

class Webserv {
	public:
		typedef std::map< std::pair<int, std::string>, ft::shared_ptr< PhysicalServer > >	PhysicalServerMap;

	private:
		static const int		MAX_SERVERS = 8;

	private:
		PhysicalServerMap	_physical_server_map;

	public:
		Webserv(void);
		Webserv(const Config &config);
		Webserv(const Webserv &ref);
		~Webserv(void);
		Webserv	&operator=(const Webserv &rhs);

	private:
		static PhysicalServerMap	_initPhysicalServerMap(const Config &config) throw(std::exception);
		static std::string			_initHost(const std::string &listen);
		static int					_initPort(const std::string &listen);

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