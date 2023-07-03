#include "../../incs/route/RouteOrAliasHandler.hpp"

RouteOrAliasHandler::RouteOrAliasHandler() {}

std::string RouteOrAliasHandler::HandleRequest(const Location &loc, const std::string &uri) {
	std::string result;
	
	if (loc.getAlias().empty() && !loc.getRoot().empty())
		result = loc.getRoot() + uri;
	else if (!loc.getAlias().empty() && loc.getRoot().empty())
		result = loc.getAlias();
	return result;
}