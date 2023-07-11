#ifndef ROUTERUTILS_HPP
# define ROUTERUTILS_HPP
#include "../../Config/ServerElement.hpp"
#include "../../Config/LocationTrieElement.hpp"
#include "../../Config/LocationElement.hpp"
#include "../../Server/VirtualServer.hpp"
#include "../../Server/VirtualServerManager.hpp"
#include "../Request/HttpRequest.hpp"

class RouterUtils {
    public:
        static std::string	findPath(ft::shared_ptr<VirtualServerManager> vsm, ft::shared_ptr<HttpRequest> req);
        static ft::shared_ptr<LocationElement> findLocation(ft::shared_ptr<VirtualServerManager> vsm, ft::shared_ptr<HttpRequest> req);
};

#endif