#ifndef AUTOINDEXELEMENT_HPP
# define AUTOINDEXELEMENT_HPP

# include "ConfigElement.hpp"

class AutoIndexElement: public ConfigElement {
	public:
		AutoIndexElement(void);
		AutoIndexElement(std::ifstream &infile) throw(std::exception);
		AutoIndexElement(const AutoIndexElement &ref);
		virtual ~AutoIndexElement(void);
		AutoIndexElement	&operator=(const AutoIndexElement &rhs);
	
	private:
		bool	_flag;
	
	private:
		bool	_parse(std::ifstream &infile) throw(std::exception);
	
	private:
		static bool	_flagIsNotValid(const std::string &flag);
	
	public:
		bool	getFlag(void) const;

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

std::ostream	&operator<<(std::ostream &os, const AutoIndexElement &rhs);

#endif