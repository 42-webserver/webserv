#include <iostream>
#include "../../libs/HandlerChain/HandlerChain.hpp"
#include "../../libs/trie/TrieTemplate.hpp"
#include "../server/Location.hpp"

// class MatchHandler : public HandlerChain {
// public:
//     MatchHandler(ft::Trie<char, Location>& trie) : trie(trie), 
// 	next_handler(new(RouteOrAliasHandler()) {}

//     std::string HandleRequest(const std::string& uri)  {
//         Location location;
//         std::string prefix = trie.searchLongestPrefix(uri);
//         if (!prefix.empty()) {
//             Location* loc = trie.getLocation(prefix);
//             if (loc) {
//                 location = *loc;
//                 return routeOrAliasHandler.HandleRequest(uri, location);
//             }
//         }

//        next_handler->HandleRequest(Location);
//         } else {
//             return "No match";
//         }
//     }

// private:
//     Trie<char, Location>& trie;
//     RouteOrAliasHandler routeOrAliasHandler;
// };

// class RouteOrAliasHandler : public HandlerChain {
// public:
//     std::string HandleRequest(const std::string& uri, const Location& location) {
//         if (!location.getAlias().empty()) {
//             return location.getAlias();
//         } else if (!location.getRoot().empty()) {
//             return location.getRoot() + uri;
//         }

//         return uri;
//     }
// };

