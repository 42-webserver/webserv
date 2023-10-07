#ifndef CGIPASSELEMENT_HPP
# define CGIPASSELEMENT_HPP

# include "ConfigElement.hpp"
# include <vector>

class CgiPassElement: public ConfigElement {
	public:
		CgiPassElement(void);
		CgiPassElement(std::ifstream &infile) throw(std::exception);
		CgiPassElement(const CgiPassElement &ref);
		virtual ~CgiPassElement(void);
		CgiPassElement	&operator=(const CgiPassElement &rhs);

	private:
		std::string	_flag;

	private:
		bool	_parse(std::ifstream &infile) throw(std::exception);
	
	private:
		static bool	_flagIsNotValid(const std::string &flag);

	public:
		const std::string	&getFlag(void) const;
		
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

std::ostream	&operator<<(std::ostream &os, const CgiPassElement &rhs);

#endif