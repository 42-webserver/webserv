#ifndef ROUTE_OR_ALIAS_HANDLER_HPP
#define ROUTE_OR_ALIAS_HANDLER_HPP

#include <string>
#include "./match.hpp"
#include "../Server/Location.hpp"
class RouteOrAliasHandler{


public:
RouteOrAliasHandler();
std::string HandleRequest(const Location &loc, const std::string &uri);

};

#endif