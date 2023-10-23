// #ifndef TRIE_HPP
// # define TRIE_HPP

// # include "../shared_ptr/shared_ptr.hpp"
// # include <map>
// # include <string>
// # include <iostream>

// namespace ft {

// template <typename T>
// class Trie {
// 	public:
// 		typedef std::map< char, Trie * >			TrieMap;
// 		typedef typename TrieMap::iterator			iterator;
// 		typedef typename TrieMap::const_iterator	const_iterator;

// 	protected:
// 		bool 	_is_end;
// 		T 		_data;
// 		TrieMap	_next;

// 	public:
// 		Trie(void);
// 		~Trie(void);

// 	public:
// 		std::pair<iterator, bool>	insert(const std::string &key, const T &data);

// 	public:
// 		T	longestPrefixSearch(const std::string &key) const;
// 		std::string	longestPrefixString(const std::string &key) const;
// 	public:
// 		iterator	begin(void);
// 		iterator	end(void);
// 		iterator	find(const std::string &key);

// 	public:
// 		const_iterator	begin(void) const;
// 		const_iterator	end(void) const;
// 		const_iterator	find(const std::string &key) const;
// };

// template <typename T>
// Trie<T>::Trie(void): _is_end(false), _data(T()), _next(TrieMap()) {}

// template <typename T>
// Trie<T>::~Trie(void) {
// 	for (iterator it=_next.begin(); it!=_next.end(); it++) {
// 		delete (it->second);
// 	}
// }

// template <typename T>
// std::pair< typename Trie<T>::iterator, bool >	Trie<T>::insert(const std::string &key, const T &data) {
// 	Trie		*cur = this;
// 	iterator	it;

// 	for (size_t	i=0; i<key.size(); i++) {
// 		if ((it = cur->_next.find(key[i])) == cur->_next.end()) {
// 			it = cur->_next.insert(std::make_pair(key[i], new Trie<T>())).first;
// 		}
// 		cur = it->second;
// 	}
// 	cur->_is_end = true;
// 	cur->_data = data;
// 	return (make_pair(it, true));
// }
// template <typename T>
// T		Trie<T>::longestPrefixSearch(const std::string &key) const {
// 	T				ret;
// 	Trie			*cur = const_cast<Trie *>(this);
// 	const_iterator	it;

// 	for (size_t	i=0; i<key.size(); i++) {
// 		if (this->_is_end) {
// 			ret = this->_data;
// 		}
// 		if ((it = cur->_next.find(key[i])) == cur->_next.end()) {
// 			return (ret);
// 		}
// 		cur = it->second;
// 	}
// 	if (cur->_is_end) {
// 		ret = cur->_data;
// 	}
// 	return (ret);
// }


// // Todo: check this Must! fix : daegulee
// template <typename T>
// std::string Trie<T>::longestPrefixString(const std::string &key) const {
//     Trie *cur = const_cast<Trie *>(this);
//     const_iterator it;
//     std::string prefix;

//     for (size_t i = 0; i < key.size(); i++) {
//         if (cur->_is_end) {
//             prefix += key[i];
//         }
//         if ((it = cur->_next.find(key[i])) == cur->_next.end()) {
//             return prefix;
//         }
//         cur = it->second;
//     }
//     if (cur->_is_end) {
//         prefix += key;
//     }
//     return prefix;
// }


// template <typename T>
// typename Trie<T>::iterator	Trie<T>::begin(void) { return (this->_next.begin()); }

// template <typename T>
// typename Trie<T>::iterator	Trie<T>::end(void) { return (this->_next.end()); }

// template <typename T>
// typename Trie<T>::iterator	Trie<T>::find(const std::string &key) {
// 	iterator	ret;

// 	for (size_t	i=0; i<key.size(); i++) {
// 		if ((ret = this->_next.find(key[i])) == this->_next.end()) {
// 			return (this->_next.end());
// 		}
// 	}
// 	return (ret);
// }

// template <typename T>
// typename Trie<T>::const_iterator	Trie<T>::begin(void) const { return (this->_next.begin()); }

// template <typename T>
// typename Trie<T>::const_iterator	Trie<T>::end(void) const { return (this->_next.end()); }

// template <typename T>
// typename Trie<T>::const_iterator	Trie<T>::find(const std::string &key) const {
// 	const_iterator	ret;

// 	for (size_t	i=0; i<key.size(); i++) {
// 		if ((ret = this->_next.find(key[i])) == this->_next.end()) {
// 			return (this->_next.end());
// 		}
// 	}
// 	return (ret);
// }

// }

// #endif

//test version: 만약 안되면 윗버전 둘다 테스트 해봐야됨 

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

	public:
		std::pair<iterator, bool>	insert(const std::string &key, const T &data);

	public:
		T	longestPrefixSearch(const std::string &key) const;
		std::string	longestPrefixString(const std::string &key) const;
	public:
		iterator	begin(void);
		iterator	end(void);
		iterator	find(const std::string &key);

	public:
		const_iterator	begin(void) const;
		const_iterator	end(void) const;
		const_iterator	find(const std::string &key) const;
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
std::pair< typename Trie<T>::iterator, bool >	Trie<T>::insert(const std::string &key, const T &data) {
	Trie		*cur = this;
	iterator	it;

	for (size_t	i=0; i<key.size(); i++) {
		if ((it = cur->_next.find(key[i])) == cur->_next.end()) {
			it = cur->_next.insert(std::make_pair(key[i], new Trie<T>())).first;
		}
		cur = it->second;
	}
	cur->_is_end = true;
	cur->_data = data;
	return (make_pair(it, true));
}
template <typename T>
T Trie<T>::longestPrefixSearch(const std::string &key) const {
    T ret;
    Trie *cur = const_cast<Trie *>(this);
    const_iterator it;

    for (size_t i=0; i<key.size(); i++) {
        if (cur->_is_end) {
            ret = cur->_data;
        }
        if ((it = cur->_next.find(key[i])) == cur->_next.end()) {
            return ret;
        }
        cur = it->second;
    }
    if (cur->_is_end) {
        ret = cur->_data;
    }
    return ret;
}

template <typename T>
std::string Trie<T>::longestPrefixString(const std::string &key) const {
    Trie *cur = const_cast<Trie *>(this);
    const_iterator it;
    std::string prefix, latestPrefix;

    for (size_t i = 0; i < key.size(); i++) {
        if ((it = cur->_next.find(key[i])) == cur->_next.end()) {
            return latestPrefix;
        }
        prefix += key[i];
        cur = it->second;
        if (cur->_is_end) {
            latestPrefix = prefix;
        }
    }
    return latestPrefix;
}


template <typename T>
typename Trie<T>::iterator	Trie<T>::begin(void) { return (this->_next.begin()); }

template <typename T>
typename Trie<T>::iterator	Trie<T>::end(void) { return (this->_next.end()); }

template <typename T>
typename Trie<T>::iterator	Trie<T>::find(const std::string &key) {
	iterator	ret;

	for (size_t	i=0; i<key.size(); i++) {
		if ((ret = this->_next.find(key[i])) == this->_next.end()) {
			return (this->_next.end());
		}
	}
	return (ret);
}

template <typename T>
typename Trie<T>::const_iterator	Trie<T>::begin(void) const { return (this->_next.begin()); }

template <typename T>
typename Trie<T>::const_iterator	Trie<T>::end(void) const { return (this->_next.end()); }

template <typename T>
typename Trie<T>::const_iterator	Trie<T>::find(const std::string &key) const {
	const_iterator	ret;

	for (size_t	i=0; i<key.size(); i++) {
		if ((ret = this->_next.find(key[i])) == this->_next.end()) {
			return (this->_next.end());
		}
	}
	return (ret);
}

}

#endif