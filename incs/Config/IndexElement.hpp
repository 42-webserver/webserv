#ifndef INDEXELEMENT_HPP
# define INDEXELEMENT_HPP

# include "ConfigElement.hpp"
# include <vector>

class IndexElement: public ConfigElement {
	public:
		IndexElement(void);
		IndexElement(std::ifstream &infile) throw(std::exception);
		IndexElement(const IndexElement &ref);
		virtual ~IndexElement(void);
		IndexElement	&operator=(const IndexElement &rhs);
	
	private:
		std::vector<std::string>	_uris;

	private:
		bool	_parse(std::ifstream &infile) throw(std::exception);
	
	private:
		static bool	_uriIsNotValid(const std::string &uri);
	
	public:
		const std::vector<std::string>	&getUris(void) const;

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