#ifndef TEST_HPP
# define TEST_HPP

# include <map>
# include <string>
# include <iostream>

namespace ft {
template <typename T>
class Trie {
private:
    bool _is_end;
    T _data;
    std::map<char, Trie> _next;

public:
    Trie(): _is_end(false) {}

    void insert(std::string str, T _data, int idx = 0) {
        if (str.back() != '/') {
            str += '/';
        }

        if (idx == str.size()) {
            _is_end = true;
            this->_data = _data;
        } else {
            _next[str[idx]].insert(str, _data, idx + 1);
        }
    }

    T getData(const std::string &str, int idx = 0) {
        if (idx == str.size()) {
            if (_is_end) {
                return _data;
            } else {
                throw std::out_of_range("Key not found: " + str);
            }
        } else {
            if (_next.count(str[idx]) == 0) {
                throw std::out_of_range("Key not found: " + str);
            } else {
                return _next[str[idx]].getData(str, idx + 1);
            }
        }
    }

    T longestPrefix(const std::string &str) {
        T result;
        std::string current;
        _longestPrefix(str, 0, current, result);
        return result;
    }

private:
    void _longestPrefix(const std::string &str, int idx, std::string &current, T &result) {
        if (_is_end) {
            result = this->_data;
        }
        if (idx == str.size() || _next.count(str[idx]) == 0) {
            return;
        } else {
            current += str[idx];
            _next[str[idx]]._longestPrefix(str, idx + 1, current, result);
            current.pop_back();
        }
    }
};
}


#endif