#ifndef LOCATIONTRIEELEMENT_HPP
# define LOCATIONTRIEELEMENT_HPP

# include "../../libs/shared_ptr/shared_ptr.hpp"
# include "../../libs/Trie/Trie.hpp"
# include "ConfigElement.hpp"
# include "LocationElement.hpp"
# include <fstream>

class LocationTrieElement: public ConfigElement {
	public:
		typedef ft::shared_ptr<LocationElement>	LocationElementPtr;
		typedef ft::Trie<LocationElementPtr>	LocationElementPtrTrie;

	public:
		LocationTrieElement(void);
		LocationTrieElement(std::ifstream &infile) throw(std::exception);
		virtual ~LocationTrieElement(void);
		LocationTrieElement(const LocationTrieElement &ref);
		LocationTrieElement	&operator=(const LocationTrieElement &rhs);

	private:
		LocationElementPtrTrie	_location_element_ptr_trie;
	
	public:
		bool				insert(std::ifstream &infile) throw(std::exception);
		LocationElementPtr	longestPrefixSearch(const std::string &path) const;
	
	private:
		bool	_parse(std::ifstream &infile) throw(std::exception);

	private:
		static bool	_dirIsNotValid(const std::string &dir);
	
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
		class FailToInsertException: public std::exception {
			public:
				virtual const char	*what(void) const throw();
		};
		class DuplicatedDirectoryException: public std::exception {
			public:
				virtual const char	*what(void) const throw();
		};
};

#endif