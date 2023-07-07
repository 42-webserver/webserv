#ifndef ERRORPAGEELEMENT_HPP
# define ERRORPAGEELEMENT_HPP

# include "ConfigElement.hpp"

class ErrorPageElement: public ConfigElement {
	public:
		ErrorPageElement(void);
		ErrorPageElement(std::ifstream &infile) throw(std::exception);
		ErrorPageElement(const ErrorPageElement &ref);
		virtual ~ErrorPageElement(void);
		ErrorPageElement	&operator=(const ErrorPageElement &rhs);
	
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

#endif