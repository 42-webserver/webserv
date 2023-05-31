#include "../../incs/route/match.hpp"

Location MatchHandler::searchLocation(const std::string uri, const ft::Trie<Location>& trie) {
    return trie.getData(uri.c_str());
}
