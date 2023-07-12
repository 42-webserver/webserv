#ifndef ALLOWMETHODELEMENT_HPP
# define ALLOWMETHODELEMENT_HPP

# define M_NONE	0x00
# define M_GET		0x01
# define M_POST	0x04
# define M_DELETE	0x10

# include "ConfigElement.hpp"
# include <map>

class AllowMethodElement: public ConfigElement {
	public:
		typedef std::string					Method;
		typedef int							Flag;
		typedef std::map<Method, Flag>		MethodMap;
		typedef MethodMap::iterator			iterator;
		typedef MethodMap::const_iterator	const_iterator;
	
	private:
		static MethodMap	_method_map;
	
	private:
		static MethodMap	_initMethodMap(void);

	public:
		AllowMethodElement(void);
		AllowMethodElement(std::ifstream &infile) throw(std::exception);
		AllowMethodElement(const AllowMethodElement &ref);
		virtual ~AllowMethodElement(void);
		AllowMethodElement	&operator=(const AllowMethodElement &rhs);
	
	private:
		Flag	_flag;
	
	private:
		bool	_parse(std::ifstream &infile) throw(std::exception);
	
	public:
		int	getFlag(void) const;

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

std::ostream	&operator<<(std::ostream &os, const AllowMethodElement &rhs);

#endif