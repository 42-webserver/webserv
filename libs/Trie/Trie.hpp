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
		typedef typename std::map< char, Trie<T> * >::iterator			iterator;
		typedef typename std::map< char, Trie<T> * >::const_iterator	const_iterator;

	protected:
		bool 						_is_end;
		T 							_data;
		std::map< char, Trie<T> * >	_next;

	public:
		Trie(void): _is_end(false), _data(), _next() {}
		~Trie(void) {
			for (iterator it=this->_next.begin(); it!=this->_next.end(); it++) {
				delete it->second;
			}
		}

		bool	insert(const std::string &str, const T &data) {
			Trie<T>		*cur = this;
			iterator	it;

			for (size_t	i=0; i<str.size(); i++) {
				if ((it = cur->_next.find(str[i])) == cur->_next.end()) {
					cur->_next[str[i]] = new Trie<T>();
					it = cur->_next.find(str[i]);
				}
				cur = it->second;
			}
			cur->_is_end = true;
			cur->_data = data;
			return (true);
		}

		T	search(const std::string &str) const {
			T				ret;
			const Trie<T>	*cur;
			const_iterator	it;

			cur = this;
			for (size_t	i=0; i<str.size(); i++) {
				if ((it = cur->_next.find(str[i])) == cur->_next.end()) {
					break;
				}
				cur = it->second;
				if (cur->_is_end) {
					ret = cur->_data;
				}
			}
			return (ret);
		}
};

}

#endif