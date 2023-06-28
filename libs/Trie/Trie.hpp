#ifndef TRIE_HPP
# define TRIE_HPP

# include "../shared_ptr/shared_ptr.hpp"
# include <map>
# include <string>
# include <iostream>

namespace ft {

template <typename T>
class Trie {
	public:
		typedef typename std::map< char, ft::shared_ptr < Trie<T> > >::iterator			iterator;
		typedef typename std::map< char, ft::shared_ptr < Trie<T> > >::const_iterator	const_iterator;

	protected:
		bool 										_is_end;
		T 											_data;
		std::map< char, ft::shared_ptr< Trie<T> > >	_next;

	public:
		Trie(void): _is_end(false), _data(), _next() {}
		~Trie(void) {}

		bool	insert(const std::string &str, const T &data) {
			ft::shared_ptr< Trie<T> >	cur = ft::shared_ptr< Trie<T> >(this);
			iterator					it;

			for (size_t	i=0; i<str.size(); i++) {
				if ((it = cur->_next.find(str[i])) == cur->_next.end()) {
					cur->_next[str[i]] = ft::shared_ptr< Trie<T> >(new Trie<T>());
					it = cur->_next.find(str[i]);
				}
				cur = it->second;
			}
			cur->_is_end = true;
			cur->_data = data;

			return (true);
		}

		ft::shared_ptr< Trie<T> >	search(const std::string &str) const {
			ft::shared_ptr< Trie<T> >	cur = ft::shared_ptr< Trie<T> >(this);
			const_iterator				it;

			for (size_t	i=0; i<str.size(); i++) {
				if ((it = cur->_next.find(str[i])) == cur->_next.end()) {
					return (ft::shared_ptr< Trie<T> >(NULL));
				}
				cur = it->second;
			}
			return (cur);
		}
};

}

#endif