#ifndef LOCATIONELEMENT_HPP
# define LOCATIONELEMENT_HPP

# include "../../libs/shared_ptr/shared_ptr.hpp"
# include "ConfigElement.hpp"
# include <fstream>
# include <vector>
# include <map>

class LocationElement: public ConfigElement {
	public:
		typedef struct s_key {
			enum e_key {
				ALIAS,
				AUTOINDEX,
				ERROR_PAGE,
				INDEX,
				RETURN,
				ROOT,
			};
		}	KEY;
		typedef std::map<std::string, KEY::e_key>	KeyMap;
		typedef ft::shared_ptr<ConfigElement>		ElementPtr;
		typedef std::map<KEY::e_key, ElementPtr>	ElementMap;
		typedef ElementMap::iterator				iterator;
		typedef ElementMap::const_iterator			const_iterator;

	private:
		static const KeyMap	_key_map;
	
	private:
		static KeyMap		_initKeyMap(void);

	public:
		LocationElement(void);
		LocationElement(std::ifstream &infile) throw(std::exception);
		virtual ~LocationElement(void);
		LocationElement(const LocationElement &ref);
		LocationElement	&operator=(const LocationElement &rhs);

	private:
		ElementMap	_element_map;
	
	public:
		iterator		find(KEY::e_key key);
		const_iterator	find(KEY::e_key key) const;
	
	public:
		ElementPtr			&operator[](KEY::e_key key);
		const ElementPtr	&operator[](KEY::e_key key) const;
	
	private:
		bool	_parse(std::ifstream &infile) throw(std::exception);
	
	private:
		static bool	_elementIsNotValid(const std::string &element);
		
	public:
		class FailToParseException: public std::exception {
			public:
				virtual const char	*what(void) const throw();
		};
		class InvalidSyntaxException: public std::exception {
			public:
				virtual const char	*what(void) const throw();
		};
		class InvalidElementException: public std::exception {
			public:
				virtual const char	*what(void) const throw();
		};
		class DuplicatedElementException: public std::exception {
			public:
				virtual const char	*what(void) const throw();
		};
		class FailToCreateElementException: public std::exception {
			public:
				virtual const char	*what(void) const throw();
		};
};

#endif