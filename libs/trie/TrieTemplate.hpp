#ifndef TRITEMPLATE_HPP
# define TRITEMPLATE_HPP
namespace ft {


template <typename T, typename Value>
struct TrieNode {
    bool isEndOfWord;
    std::map<T, ft::unique_ptr<TrieNode<T, Value>>> children;
    ft::unique_ptr<Value> value;  // Store the actual data here.

    TrieNode() : isEndOfWord(false) {}
};

template <typename T, typename Value>
class Trie {
private:
    ft::unique_ptr<TrieNode<T, Value>> root;

public:
    Trie() : root(new TrieNode<T, Value>()) {}

    void insert(const std::basic_string<T> &key, const Value &value) {
        TrieNode<T, Value>* current = root.get();
        for (int i = 0; i < key.length(); ++i) {
            T c = key[i];
            if (current->children.find(c) == current->children.end())
                current->children[c].reset(new TrieNode<T, Value>());
            current = current->children[c].get();
        }
        current->isEndOfWord = true;
        current->value.reset(new Value(value)); 
    }

    Value* search(const std::basic_string<T> &key) {
        TrieNode<T, Value>* current = root.get();
        for (int i = 0; i < key.length(); ++i) {
            T c = key[i];
            if (current->children.find(c) == current->children.end())
                return NULL;
            current = current->children[c].get();
        }
        return current != NULL && current->isEndOfWord ? current->value.get() : NULL;
    }
};

}
#endif