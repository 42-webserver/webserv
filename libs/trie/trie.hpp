#ifndef TRIE_HPP
# define TRIE_HPP

# include <map>
# include <stdexcept>
# include <string>
# include <iostream>

namespace ft {

template <typename T>
class Trie {
	public:
		typedef typename std::map<char, Trie<T> *>::iterator		iterator;
		typedef typename std::map<char, Trie<T> *>::const_iterator	const_iterator;

	private:
		std::map<char, Trie<T> *>	_next;
		bool						_is_end;
		T							_data;

	public:
		Trie(void);
		~Trie(void);

	private:
		Trie(const Trie &ref);
		Trie	&operator=(const Trie &rhs);

	private:
		const Trie	*_find_longest_prefix(const char *str) const;

	public:
		void	insert(const char *str, T data);
		const T	&getData(const char *str) const throw(std::exception);
};

template <typename T>
Trie<T>::Trie(void): _next(), _is_end(false), _data() {}

template <typename T>
Trie<T>::~Trie(void) {
	for (int i = 0; i < 26; i++) {
		if (_next[i] != NULL) {
			delete _next[i];
		}
	}
}

template <typename T>
Trie<T>::Trie(const Trie &ref): _next(ref._next), _is_end(ref._is_end), _data(ref._data) {}

template <typename T>
Trie<T>	&Trie<T>::operator=(const Trie &rhs) {
	if (this != &rhs) {
		this->~Trie();
		new (this) Trie(rhs);
	}
	return (*this);
}

template <typename T>
const Trie<T>	*Trie<T>::_find_longest_prefix(const char *str) const {
	Trie<T>	*ret = NULL;

	if (*str == '\0' && this->_is_end) {
		return (this);
	} else if (*str == '\0' && !this->_is_end) {
		return (NULL);
	}

	for (const_iterator it=this->_next.begin(); it!=this->_next.end(); it++) {
		if (it->first != *str) {
			continue;
		}
		ret = (Trie<T> *) it->second->_find_longest_prefix(str + 1);
		if (ret == NULL && it->second->_is_end) {
			ret = (Trie<T> *) it->second;
		}
		break ;
	}

	return (ret);
}

template <typename T>
void	Trie<T>::insert(const char *str, T data) {
	if (this->_next[*str] == NULL) {
		this->_next[*str] = new Trie<T>();
	}

	if (*(str + 1) == '\0') {
		if (*str != '/') {
			this->_next[*str]->insert("/", data);
		} else {
			this->_next[*str]->_is_end = true;
			this->_next[*str]->_data = data;
		}
		return;
	}
	this->_next[*str]->insert(str + 1, data);
}

template <typename T>
const T	&Trie<T>::getData(const char *str) const  throw(std::exception) {
	std::string		target = std::string(str);
	const Trie<T>	*cur;

	if (target.back() != '/') {
		target += '/';
	}

	cur = this->_find_longest_prefix(target.c_str());

	if (cur == NULL) {
		std::string	msg = std::string("Trie::getData: key not found: ") + str;
		throw std::out_of_range(msg);
	}

	return (cur->_data);
}

}

#endif