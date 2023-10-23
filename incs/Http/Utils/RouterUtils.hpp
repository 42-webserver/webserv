#ifndef ROUTERUTILS_HPP
# define ROUTERUTILS_HPP
#include "../../Config/ServerElement.hpp"
#include "../../Config/LocationTrieElement.hpp"
#include "../../Config/IndexElement.hpp"
#include "../../Config/LocationElement.hpp"
#include "../../Config/RootElement.hpp"
#include "../../Config/AllowMethodElement.hpp"
#include "../../Config/AliasElement.hpp"
#include "../../Config/AutoIndexElement.hpp"
#include "../../Server/VirtualServer.hpp"
#include "../../Server/VirtualServerManager.hpp"
#include "../Request/HttpRequest.hpp"
#include "../../Config/ClientMaxBodySizeElement.hpp"
#include "../../Config/CgiPassElement.hpp"
// #include "../Utils/FileUploader.hpp"
#include "../Utils/HttpMethod.hpp"
#include "../Exception/NotFoundException.hpp"
class RouterUtils {
public:
    static std::string	findPath(ft::shared_ptr<VirtualServerManager> vsm, ft::shared_ptr<HttpRequest> req);
    static std::string	findPriorityPathWithIndex(ft::shared_ptr<VirtualServerManager> vsm, ft::shared_ptr<HttpRequest> req);
    static std::string  findRedirectUri(ft::shared_ptr<VirtualServerManager> vsm, ft::shared_ptr<HttpRequest> req);
    static ft::shared_ptr<LocationElement> findLocation(ft::shared_ptr<HttpRequest> req,
    ft::shared_ptr<VirtualServer> targetServer);
    static int          findMaxBodySize(ft::shared_ptr<VirtualServerManager> vsm, ft::shared_ptr<HttpRequest> req);
    static bool         isCgiRequest(ft::shared_ptr<VirtualServerManager> vsm, ft::shared_ptr<HttpRequest> req);

    static bool         isMethodAllowed(ft::shared_ptr<VirtualServerManager> vsm, ft::shared_ptr<HttpRequest> req);
    static bool         isRedirection(ft::shared_ptr<VirtualServerManager> vsm, ft::shared_ptr<HttpRequest> req);
    static std::string findCgiScriptPath(ft::shared_ptr<VirtualServerManager> vsm, ft::shared_ptr<HttpRequest> req);

    static std::string findPathInfo(ft::shared_ptr<VirtualServerManager> vsm, ft::shared_ptr<HttpRequest> req);
private:
    static std::string	_findRoot(ft::shared_ptr<VirtualServerManager> vsm, ft::shared_ptr<HttpRequest> req);
    static std::string	_findAlias(ft::shared_ptr<VirtualServerManager> vsm, ft::shared_ptr<HttpRequest> req);
    static std::string  _makePath(std::string &root, std::string &alias, std::string &uri);
    static std::string _findIndex(ft::shared_ptr<VirtualServerManager> vsm, ft::shared_ptr<HttpRequest> req, std::string path);
    static std::string _findIndexInLocation(std::string path, ft::shared_ptr<LocationElement> locationElement);
    static std::string _findIndexInServer(std::string path, ft::shared_ptr<VirtualServerManager> vsm, ft::shared_ptr<HttpRequest> req);
    static ft::shared_ptr<VirtualServer> _findVirtualServer(ft::shared_ptr<VirtualServerManager> vsm, ft::shared_ptr<HttpRequest> req);
};  

#endif