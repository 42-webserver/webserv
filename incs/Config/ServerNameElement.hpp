#ifndef SERVERNAMEELEMENT_HPP
# define SERVERNAMEELEMENT_HPP

# include "ConfigElement.hpp"
# include <string>
# include <vector>

class ServerNameElement: public ConfigElement {
	public:
		typedef std::string	ServerName;
	
	private:
		std::vector<ServerName>	_server_names;
	
	public:
		ServerNameElement(void);
		ServerNameElement(std::ifstream &infile) throw(std::exception);
		virtual ~ServerNameElement(void);
		ServerNameElement(const ServerNameElement &ref);
		ServerNameElement	&operator=(const ServerNameElement &rhs);

	private:
		bool	_parse(std::ifstream &infile) throw(std::exception);
	
	private:
		static bool	_nameIsNotValid(const ServerName &name);
	
	public:
		const std::vector<ServerName>	&getServerNames(void) const;

	public:
		class FailToParseException: public std::exception {
			public:
				virtual const char	*what(void) const throw();
		};
		class InvalidSyntaxException: public std::exception {
			public:
				virtual const char	*what(void) const throw();
		};
};

std::ostream	&operator<<(std::ostream &os, const ServerNameElement &rhs);

#endif