#ifndef ROUTERUTILS_HPP
# define ROUTERUTILS_HPP
#include "../../Config/ServerElement.hpp"
#include "../../Config/LocationTrieElement.hpp"
#include "../../Config/IndexElement.hpp"
#include "../../Config/LocationElement.hpp"
#include "../../Config/RootElement.hpp"
#include "../../Config/AliasElement.hpp"
#include "../../Server/VirtualServer.hpp"
#include "../../Server/VirtualServerManager.hpp"
#include "../Request/HttpRequest.hpp"
#include "../../Config/ClientMaxBodySizeElement.hpp"
#include "../Utils/FileUploader.hpp"
#include "../Exception/NotFoundException.hpp"

class RouterUtils {
public:
    static std::string	findPath(ft::shared_ptr<VirtualServerManager> vsm, ft::shared_ptr<HttpRequest> req);
    static std::string	findPriorityPathWithIndex(ft::shared_ptr<VirtualServerManager> vsm, ft::shared_ptr<HttpRequest> req) throw (NotFoundException);
    static ft::shared_ptr<LocationElement> findLocation(ft::shared_ptr<VirtualServerManager> vsm, ft::shared_ptr<HttpRequest> req);
    static int            findMaxBodySize(ft::shared_ptr<VirtualServerManager> vsm, ft::shared_ptr<HttpRequest> req);
private:
    static std::string	_findRoot(ft::shared_ptr<LocationElement> locationElement);
    static std::string	_findAlias(ft::shared_ptr<LocationElement> locationElement);
    static std::string  _makePath(std::string &root, std::string &alias, std::string &uri);
};

#endif