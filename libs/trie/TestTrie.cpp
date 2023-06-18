#include "test.hpp"
#include <iostream>

int	main(void) {
	ft::Trie<int>	trie;

	trie.insert("/", 0);
	trie.insert("/abc", 1);
	trie.insert("/abc/de", 2);
	trie.insert("/abc/def", 3);

	try {
		// std::cout << trie.getData("/index.html") << '\n';
		// std::cout << trie.getData("/ab/index.html") << '\n';
		// std::cout << trie.getData("/abc/d/index.html") << '\n';
		// std::cout << trie.getData("/abc/de/index.html") << '\n';
		// std::cout << trie.getData("/abc/def/index.html") << '\n';
		// std::cout << trie.getData("/abc/deg/index.html") << '\n';
		std::cout << trie.longestPrefix("/index.html") << '\n';
		std::cout << trie.longestPrefix("/ab/index.html") << '\n';
		std::cout << trie.longestPrefix("/abc/d/index.html") << '\n';
		std::cout << trie.longestPrefix("/abc/de/index.html") << '\n';
		std::cout << trie.longestPrefix("/abc/def/index.html") << '\n';
		std::cout << trie.longestPrefix("/abc/deg/index.html") << '\n';		
	} catch (const std::exception &e) {
		std::cout << "Error: " << e.what() << '\n';
	}
	return (0);
}