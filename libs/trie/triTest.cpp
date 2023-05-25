#include <iostream>
#include <cassert>
#include "trie.hpp"

void foo(){
	ft::Trie trie;

    trie.insert("/home");
    trie.insert("/home/user");
    trie.insert("/home/user/profile");
    trie.insert("/about");
    trie.insert("/about/company");

    assert(trie.search("/home") == true);
    assert(trie.search("/home/user") == true);
    assert(trie.search("/home/user/profile") == true);
    assert(trie.search("/about") == true);
    assert(trie.search("/about/company") == true);
    assert(trie.search("/contact") == false);
    assert(trie.search("/home/user/profile/edit") == false);

    std::cout << "All tests passed!\n";

}
int main() {
    foo();
    system("leaks a.out");
    std::cout << "All tests passed!\n";

    return 0;
}
