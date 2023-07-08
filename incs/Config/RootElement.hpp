#ifndef ROOTELEMENT_HPP
# define ROOTELEMENT_HPP

# include "ConfigElement.hpp"
# include <vector>

class RootElement: public ConfigElement {
	public:
		RootElement(void);
		RootElement(std::ifstream &infile) throw(std::exception);
		RootElement(const RootElement &ref);
		virtual ~RootElement(void);
		RootElement	&operator=(const RootElement &rhs);

	private:
		std::string	_path;

	private:
		bool	_parse(std::ifstream &infile) throw(std::exception);
	
	private:
		static bool	_pathIsNotValid(const std::string &path);

	public:
		const std::string	&getPath(void) const;
		
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

std::ostream	&operator<<(std::ostream &os, const RootElement &rhs);

#endif