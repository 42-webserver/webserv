#include "./trie.hpp"

ft::Trie::Trie() : root(new TrieNode()) {}

bool ft::Trie::search(const std::string &key) {
        TrieNode* current = root.get();
        for (int i = 0; i < key.length(); ++i) {
            char c = key[i];
            if (current->children.find(c) == current->children.end())
                return false;
            current = current->children[c].get();
        }
        return current != NULL && current->isEndOfWord;
    }

void ft::Trie::insert(const std::string &key) {
       TrieNode* current = root.get();
       for (int i = 0; i < key.length(); ++i) {
           char c = key[i];
           if (current->children.find(c) == current->children.end())
               current->children[c].reset(new TrieNode());
           current = current->children[c].get();
       }
       current->isEndOfWord = true;
   }