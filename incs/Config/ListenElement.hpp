#ifndef LISTENELEMENT_HPP
# define LISTENELEMENT_HPP

# include "ConfigElement.hpp"
# include <string>

class ListenElement: public ConfigElement {
	public:
		typedef std::string	Host;
		typedef std::string	Ip;
		typedef int			Port;

	public:
		ListenElement(void);
		ListenElement(std::ifstream &infile);
		virtual ~ListenElement(void);
		ListenElement(const ListenElement &ref);
		ListenElement	&operator=(const ListenElement &rhs);

	private:
		Ip		_ip;
		Port	_port;
	
	private:
		bool	_parse(std::ifstream &infile) throw(std::exception);
	
	private:
		static bool	_ipIsNotValid(const Ip &ip);
		static bool	_portIsNotValid(const Port &port);
	
	public:
		const Ip	&getIp(void) const;
		const Port	&getPort(void) const;
	
	public:
		class FailToParseException: public std::exception {
			public:
				virtual const char	*what(void) const throw();
		};
		class InvalidArgumentException: public std::exception {
			public:
				virtual const char	*what(void) const throw();
		};
		class InvalidSyntaxException: public std::exception {
			public:
				virtual const char	*what(void) const throw();
		};
		class InvalidIpException: public std::exception {
			public:
				virtual const char	*what(void) const throw();
		};
		class InvalidPortException: public std::exception {
			public:
				virtual const char	*what(void) const throw();
		};
};

std::ostream	&operator<<(std::ostream &os, const ListenElement &rhs);

#endif