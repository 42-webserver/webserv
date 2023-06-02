#ifndef MATCH_HPP
#define MATCH_HPP

#include <iostream>
#include "../../libs/Trie/Trie.hpp"
#include "./RouteOrAliasHandler.hpp"
#include "../Server/Location.hpp"

class MatchHandler {
private:
public:
	static Location searchLocation(const std::string uri, const ft::Trie<Location>& trie);
};

#endif

