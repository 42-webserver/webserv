#ifndef TRITEMPLATE_HPP
# define TRITEMPLATE_HPP
namespace ft {

template <typename T, typename Value>
struct TrieNode {
    bool isEndOfWord;
    std::map<T, ft::unique_ptr<TrieNode<T, Value> > > children;
    ft::unique_ptr<Value> value;

    TrieNode() : isEndOfWord(false) {}
};

template <typename T, typename Value>
class Trie {
private:
    ft::unique_ptr<TrieNode<T, Value> > root;

public:
    Trie() : root(new TrieNode<T, Value>()) {}

    void insert(const T* key, size_t length, const Value& value) {
        TrieNode<T, Value>* current = root.get();
        for (size_t i = 0; i < length; ++i) {
            T c = key[i];
            typename std::map<T, ft::unique_ptr<TrieNode<T, Value> > >::iterator it = current->children.find(c);
            if (it == current->children.end())
                current->children[c].reset(new TrieNode<T, Value>());
            current = current->children[c].get();
        }
        current->isEndOfWord = true;
        current->value.reset(new Value(value));
    }

    Value* search(const T* key, size_t length) {
        TrieNode<T, Value>* current = root.get();
        for (size_t i = 0; i < length; ++i) {
            T c = key[i];
            typename std::map<T, ft::unique_ptr<TrieNode<T, Value> > >::iterator it = current->children.find(c);
            if (it == current->children.end())
                return NULL;
            current = it->second.get();
        }
        return current != NULL && current->isEndOfWord ? current->value.get() : NULL;
    }
};

}
#endif