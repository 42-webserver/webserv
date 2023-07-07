#ifndef ALIASELEMENT_HPP
# define ALIASELEMENT_HPP

# include "ConfigElement.hpp"
# include <vector>

class AliasElement: public ConfigElement {
	public:
		AliasElement(void);
		AliasElement(std::ifstream &infile) throw(std::exception);
		AliasElement(const AliasElement &ref);
		virtual ~AliasElement(void);
		AliasElement	&operator=(const AliasElement &rhs);

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

#endif