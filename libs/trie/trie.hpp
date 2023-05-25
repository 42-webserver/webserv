#ifndef TRI_HPP
# define TRI_HPP

#include <map>
#include <string>
#include "../unique_ptr/unique_ptr.hpp"

namespace ft {

struct TrieNode {
    bool isEndOfWord;
    std::map<char, ft::unique_ptr<TrieNode> > children;
    TrieNode() : isEndOfWord(false) {}
};

class Trie {
private:
    ft::unique_ptr<TrieNode> root;

public:
    Trie();

    void insert(const std::string &key);

    bool search(const std::string &key);
};
}
#endif