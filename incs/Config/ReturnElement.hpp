#ifndef RETURNELEMENT_HPP
# define RETURNELEMENT_HPP

# include "ConfigElement.hpp"
# include <vector>

class ReturnElement: public ConfigElement {
	public:
		ReturnElement(void);
		ReturnElement(std::ifstream &infile) throw(std::exception);
		ReturnElement(const ReturnElement &ref);
		virtual ~ReturnElement(void);
		ReturnElement	&operator=(const ReturnElement &rhs);

	private:
		int			_code;
		std::string	_uri;	

	private:
		bool	_parse(std::ifstream &infile) throw(std::exception);
	
	private:
		static bool	_codeIsNotValid(int code);
		static bool	_uriIsNotValid(const std::string &uri);

	public:
		int					getCode(void) const;
		const std::string	&getUri(void) const;
		
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

std::ostream	&operator<<(std::ostream &os, const ReturnElement &rhs);

#endif