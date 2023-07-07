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
		typedef std::map< char, Trie * >			TrieMap;
		typedef typename TrieMap::iterator			iterator;
		typedef typename TrieMap::const_iterator	const_iterator;

	protected:
		bool 	_is_end;
		T 		_data;
		TrieMap	_next;

	public:
		Trie(void);
		~Trie(void);

		bool	insert(const std::string &key, const T &data);
		Trie<T>	*find(const std::string &key) const;
		T		longestPrefixSearch(const std::string &key) const;
};

template <typename T>
Trie<T>::Trie(void): _is_end(false), _data(T()), _next(TrieMap()) {}

template <typename T>
Trie<T>::~Trie(void) {
	for (iterator it=_next.begin(); it!=_next.end(); it++) {
		delete (it->second);
	}
}

template <typename T>
bool	Trie<T>::insert(const std::string &key, const T &data) {
	Trie		*cur = this;
	iterator	it;

	for (size_t	i=0; i<key.size(); i++) {
		if ((it = cur->_next.find(key[i])) == cur->_next.end()) {
			cur->_next[key[i]] = new Trie<T>();
			it = cur->_next.find(key[i]);
		}
		cur = it->second;
	}
	cur->_is_end = true;
	cur->_data = data;
	return (true);
}
template <typename T>
Trie<T>	*Trie<T>::find(const std::string &key) const {
	Trie			*ret = const_cast<Trie *>(this);
	const_iterator	it;

	for (size_t	i=0; i<key.size(); i++) {
		if ((it = ret->_next.find(key[i])) == ret->_next.end()) {
			return (NULL);
		}
		ret = it->second;
	}
	return (ret);
}
template <typename T>
T		Trie<T>::longestPrefixSearch(const std::string &key) const {
	T				ret;
	Trie			*cur = this;
	const_iterator	it;

	for (size_t	i=0; i<key.size(); i++) {
		if (this->_is_end) {
			ret = this->_data;
		}
		if ((it = cur->_next.find(key[i])) == cur->_next.end()) {
			return (ret);
		}
		cur = it->second;
	}
	if (cur->_is_end) {
		ret = cur->_data;
	}
	return (ret);
}

}

#endif