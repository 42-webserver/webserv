#ifndef CLIENTMAXBODYSIZEELEMENT_HPP
# define CLIENTMAXBODYSIZEELEMENT_HPP

# include "ConfigElement.hpp"

class ClientMaxBodySizeElement: public ConfigElement {
	public:
		ClientMaxBodySizeElement(void);
		ClientMaxBodySizeElement(std::ifstream &infile) throw(std::exception);
		ClientMaxBodySizeElement(const ClientMaxBodySizeElement &ref);
		virtual ~ClientMaxBodySizeElement(void);
		ClientMaxBodySizeElement	&operator=(const ClientMaxBodySizeElement &rhs);
	
	private:
		int		_num;
		char	_unit;
	
	private:
		bool	_parse(std::ifstream &infile) throw(std::exception);
	
	private:
		static bool	_numIsNotValid(int num);
		static bool	_unitIsNotValid(char unit);

	public:
		int		getNum(void) const;
		char	getUnit(void) const;

	public:
		class FailToParseException: public std::exception {
			public:
				virtual const char	*what(void) const throw();
		};
		class InvalidSyntaxException: public std::exception {
			public:
				virtual const char	*what(void) const throw();
		};
		class InvalidArgumentException: public std::exception {
			public:
				virtual const char	*what(void) const throw();
		};
};

std::ostream	&operator<<(std::ostream &os, const ClientMaxBodySizeElement &rhs);

#endif