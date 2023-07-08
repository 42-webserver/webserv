#ifndef SERVERELEMENT_HPP
# define SERVERELEMENT_HPP

# include "../../libs/shared_ptr/shared_ptr.hpp"
# include "ConfigElement.hpp"
# include <fstream>
# include <vector>
# include <map>

class ServerElement: public ConfigElement {
	public:
		typedef struct s_key {
			enum e_key {
				AUTOINDEX,
				CLIENT_MAX_BODY_SIZE,
				ERROR_PAGE,
				INDEX,
				LISTEN,
				LOCATION,
				LOCATION_TRIE,
				RETURN,
				ROOT,
				SERVER,
				SERVER_NAME,
			};
		}	KEY;
		typedef std::map<std::string, KEY::e_key>	KeyMap;
		typedef ft::shared_ptr<ConfigElement>		ElementPtr;
		typedef std::map<KEY::e_key, ElementPtr >	ElementMap;
		typedef ElementMap::iterator				iterator;
		typedef ElementMap::const_iterator			const_iterator;

	private:
		static const KeyMap	_key_map;
	
	private:
		static KeyMap		_initKeyMap(void);

	public:
		ServerElement(void);
		ServerElement(std::ifstream &infile) throw(std::exception);
		virtual ~ServerElement(void);
		ServerElement(const ServerElement &ref);
		ServerElement	&operator=(const ServerElement &rhs);

	private:
		ElementMap	_element_map;
	
	public:
		iterator			begin(void);
		iterator			end(void);
		iterator			find(KEY::e_key key);
	
	public:
		const_iterator		begin(void) const;
		const_iterator		end(void) const;
		const_iterator		find(KEY::e_key key) const;
	
	public:
		ElementPtr			&operator[](KEY::e_key key);
		const ElementPtr	&operator[](KEY::e_key key) const;
	
	private:
		bool	_parse(std::ifstream &infile) throw(std::exception);
		
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

std::ostream	&operator<<(std::ostream &os, const ServerElement &rhs);

#endif